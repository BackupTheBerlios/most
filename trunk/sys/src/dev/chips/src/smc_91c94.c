/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stddef.h>
#include <uso/semaphore.h>
#include <uso/io_ports.h>
#include <uso/thread.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <uso/heap.h>
#include <dev/cpu.h>
#include <dev/arch/h83003/bitops.h>
#include <dev/arch/h83003/H83003_REG.h>
#include <dev/arch/h83003/H83003.h>

#include <dev/chips/smc_91c94.h>
#include <dev/chips/smc_91c94_def.h>
#include <dev/debug.h>
#include <net/netif.h>
#include <net/ethif.h>

#define SMC_SELECT_BANK(x)  { USO_out_w(smc->io_addr + SMC_BANK_SELECT, x); }

#if MOST_CPU_H8300

#define ETHIRQ_ON()   H8_BITSET( H8_IER_IRQ7E, H8_IER)
#define ETHIRQ_OFF()  H8_BITCLEAR( H8_IER_IRQ7E, H8_IER)
#define ETHIRQ_QUIT() H8_BITCLEAR(H8_ISR_IRQ7F, H8_ISR)

#else

#define ETHIRQ_ON()
#define ETHIRQ_OFF()
#define ETHIRQ_QUIT()

#endif

/*
 * enables an interrupt in the interrupt mask register 
 */
static void
smc_enable_int (unsigned char x, DEV_smc91c94_t * smc)
{
    unsigned char mask;
    ETHIRQ_OFF ();
    SMC_SELECT_BANK (2);
    mask = USO_in_b (smc->io_addr + SMC_MSK);
    mask |= x;
    USO_out_b (smc->io_addr + SMC_MSK, mask);
    ETHIRQ_ON ();
}

/*
 * disables an interrupt in the interrupt mask register 
 */
static void
smc_disable_int (unsigned char x, DEV_smc91c94_t * smc)
{
    unsigned char mask;
    ETHIRQ_OFF ();
    SMC_SELECT_BANK (2);
    mask = USO_in_b (smc->io_addr + SMC_MSK);
    mask &= ~x;
    USO_out_b (smc->io_addr + SMC_MSK, mask);
    ETHIRQ_ON ();
}

static void
smc_soft_reset (DEV_smc91c94_t * smc)
{
    SMC_SELECT_BANK (0);
    USO_out_w (smc->io_addr + SMC_RCR, SMC_RCR_SOFT_RST);
    DEV_cpudelay (DEV_USEC_2_LOOPS (100));
}

/*
 * get input states of IOS0-IOS2 from smc chip 
 */
static void
smc_check_ios (DEV_smc91c94_t * smc)
{
    unsigned char smc_io;
    SMC_SELECT_BANK (3);
    smc_io = (USO_in_w (smc->io_addr + SMC_MGMT) & 0x0700) >> 8;
    DEBUGF (DEV_SMC_DEBUG, ("Smc: io %02x.\n", smc_io));
}

/*
 * Write the MAC address ( bank 1, regs 4 - 9 ) 
 */
static void
smc_set_mac_addr (DEV_smc91c94_t * smc)
{
    int i;
    SMC_SELECT_BANK (1);
    for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
    {
        USO_out_b (smc->io_addr + SMC_IAR0 + i, smc->mac_addr[i]);
    }
}

static void
smc_start (DEV_smc91c94_t * smc)
{
    ETHIRQ_OFF ();

    smc->link_is_up = TRUE;
    smc_soft_reset (smc);
    smc_check_ios (smc);
    smc_set_mac_addr (smc);

    /*
     * Set the transmit and receive configuration registers to default
     * values 
     */
    SMC_SELECT_BANK (0);
    USO_out_w (smc->io_addr + SMC_RCR, SMC_RCR_CLEAR);
    USO_out_w (smc->io_addr + SMC_TCR, SMC_TCR_CLEAR);

    /*
     * set the control register to automatically release successfully
     * transmitted packets, to make the best use out of our limited memory 
     */
    SMC_SELECT_BANK (1);
    USO_out_w (smc->io_addr + SMC_CTR, SMC_CTR_NORMAL);

    /*
     * select 10BaseT port !!! 
     */
    USO_out_w (smc->io_addr + SMC_CR, (USO_in_w (smc->io_addr + SMC_CR)) & ~SMC_CR_AUI_SELECT);

    /*
     * Reset the MMU 
     */
    SMC_SELECT_BANK (2);
    USO_out_w (smc->io_addr + SMC_MMUCR, SMC_MMUCR_RESET);

    /*
     * Note: It doesn't seem that waiting for the MMU busy is needed here,
     * but this is a place where future chipsets _COULD_ break.  Be wary of 
     * issuing another MMU command right after this 
     */

    USO_out_b (smc->io_addr + SMC_MSK, 0);

    SMC_SELECT_BANK (0);
    /*
     * see the header file for options in TCR/RCR NORMAL 
     */
    USO_out_w (smc->io_addr + SMC_TCR, SMC_TCR_NORMAL);
    USO_out_w (smc->io_addr + SMC_RCR, SMC_RCR_NORMAL);

    /*
     * now, enable interrupts 
     */
    SMC_SELECT_BANK (2);
    USO_out_b (smc->io_addr + SMC_MSK, SMC_INT_MASK);

    ETHIRQ_QUIT ();
    ETHIRQ_ON ();
}



static void
receive_int (DEV_smc91c94_t * smc)
{
    USO_signal (&smc->rx_sem);  // signal packet received
}

static void
rx_overrun_int (DEV_smc91c94_t * smc)
{
    DEBUGF (DEV_SMC_DEBUG, ("Smc: rx overrun int.\n"));
    ++smc->rx_overruns;
}

static void
early_rx_int (DEV_smc91c94_t * smc)
{
    DEBUGF (DEV_SMC_DEBUG, ("Smc: early rx int.\n"));
    ++smc->ercv_ints;
}

/*************************************************************************
 . smc_tx
 .
 . Purpose:  Handle a transmit error message.   This will only be called
 .   when an error, because of the AUTO_RELEASE mode.
 .
 . Algorithm:
 .  Save pointer and packet no
 .  Get the packet no from the top of the queue
 .  check if it's valid ( if not, is this an error??? )
 .  read the status word
 .  record the error
 .  ( resend?  Not really, since we don't want old packets around )
 .  Restore saved values
 ************************************************************************/
static void
smc_tx_error (DEV_smc91c94_t * smc)
{
    unsigned char saved_packet;
    unsigned char packet_no;
    unsigned short tx_status;

    ++smc->tx_errors;

    // assume bank 2

    saved_packet = USO_in_b (smc->io_addr + SMC_PNR);
    packet_no = USO_in_w (smc->io_addr + SMC_FIFO);
    packet_no &= 0x7F;

    // select this as the packet to read from
    USO_out_b (smc->io_addr + SMC_PNR, packet_no);

    // read the first word from this packet
    USO_out_w (smc->io_addr + SMC_PTR, SMC_PTR_AUTOINCR | SMC_PTR_READ);

    tx_status = USO_in_w (smc->io_addr + SMC_DATA1);

    // re-enable transmit
    SMC_SELECT_BANK (0);
    USO_out_w (smc->io_addr + SMC_TCR, USO_in_w (smc->io_addr + SMC_TCR) | SMC_TCR_TX_ENA);

    // kill the packet
    SMC_SELECT_BANK (2);
    USO_out_w (smc->io_addr + SMC_MMUCR, SMC_MMUCR_FREEPKT);

    USO_out_b (smc->io_addr + SMC_PNR, saved_packet);
    return;
}

static void
transmit_int (DEV_smc91c94_t * smc)
{
    DEBUGF (DEV_SMC_DEBUG, ("Smc: tx int.\n"));
    smc_tx_error (smc);
}

static void
tx_empty_int (DEV_smc91c94_t * smc)
{
    DEBUGF (DEV_SMC_DEBUG, ("Smc: tx empty int.\n"));
    ++smc->ercv_ints;
}

static void
alloc_int (DEV_smc91c94_t * smc)
{
    DEBUGF (DEV_SMC_DEBUG, ("Smc: alloc int.\n"));
    ++smc->alloc_ints;
}

static void
eph_int (DEV_smc91c94_t * smc)
{
    unsigned short tmp;
    DEBUGF (DEV_SMC_DEBUG, ("Smc: eph int.\n"));

    ++smc->eph_ints;

    /*
     * Ack eph int 
     */
    SMC_SELECT_BANK (1);
    USO_out_w (smc->io_addr + SMC_CTR, SMC_CTR_CLEAR);
    USO_out_w (smc->io_addr + SMC_CTR, SMC_CTR_NORMAL);

    SMC_SELECT_BANK (0);
    tmp = USO_in_w (smc->io_addr + SMC_EPHSR);

    if ((tmp & SMC_EPHSR_LINK_OK) == 0)
    {
        smc->link_is_up = FALSE;
        ++smc->link_down_count;
        DEBUGF (DEV_SMC_DEBUG, ("Smc: link is down.\n"));
    }
    else
    {
        smc->link_is_up = TRUE;
        DEBUGF (DEV_SMC_DEBUG, ("Smc: link is up.\n"));
    }

    // do MAC statistic
    tmp = USO_in_w (smc->io_addr + SMC_ECR);
    smc->tx_exc_def += (tmp & 0xf000) >> 12;
    smc->tx_def += (tmp & 0x0f00) >> 8;
    smc->tx_mcc += (tmp & 0x00f0) >> 4;
    smc->tx_scc += tmp & 0x000f;

    tmp = USO_in_w (smc->io_addr + SMC_TCR);
    if ((tmp & SMC_TCR_TX_ENA) == 0)
    {
        USO_out_w (smc->io_addr + SMC_TCR, SMC_TCR_NORMAL);
    }
}

extern void
DEV_smc_interrupt (DEV_smc91c94_t * smc)
{
    unsigned short saved_bank_select;
    unsigned short saved_pointer;
    unsigned char status;

    ETHIRQ_QUIT ();

    saved_bank_select = USO_in_w (smc->io_addr + SMC_BANK_SELECT);
    SMC_SELECT_BANK (2);
    saved_pointer = USO_in_w (smc->io_addr + SMC_PTR);
    status = USO_in_b (smc->io_addr + SMC_IST);

    if (status & SMC_IM_RCV_INT)
    {
        smc_disable_int (SMC_IM_RCV_INT, smc);
        receive_int (smc);
    }
    else if (status & SMC_IAM_TX_INT)
    {
        smc_disable_int (SMC_IAM_TX_INT, smc);
        USO_out_b (smc->io_addr + SMC_ACK, SMC_IAM_TX_INT);
        transmit_int (smc);
    }
    else if (status & SMC_IM_EPH_INT)
    {
        eph_int (smc);
    }
    else if (status & SMC_IAM_RX_OVRN_INT)
    {
        USO_out_b (smc->io_addr + SMC_ACK, SMC_IAM_RX_OVRN_INT);
        rx_overrun_int (smc);
    }
    else if (status & SMC_IAM_ERCV_INT)
    {
        USO_out_b (smc->io_addr + SMC_ACK, SMC_IAM_ERCV_INT);
        early_rx_int (smc);
    }
    else if (status & SMC_IAM_TX_EMPTY_INT)
    {
        smc_disable_int (SMC_IAM_TX_EMPTY_INT, smc);
        USO_out_b (smc->io_addr + SMC_ACK, SMC_IAM_TX_EMPTY_INT);
        tx_empty_int (smc);
    }
    else if (status & SMC_IM_ALLOC_INT)
    {
        smc_disable_int (SMC_IM_ALLOC_INT, smc);
        alloc_int (smc);
    }

    USO_out_w (smc->io_addr + SMC_PTR, saved_pointer);
    USO_out_w (smc->io_addr + SMC_BANK_SELECT, saved_bank_select);
}


/*------------------------------------------------------------------------
 *  ethread - read a single packet from the ethernet
 *------------------------------------------------------------------------
 */
static NET_netbuf_t *
smc_rx_read_data (DEV_smc91c94_t * smc)
{
    /*-------------------------------------------------------------
      .
      . smc_rcv -  receive a packet from the card
      .
      . There is ( at least ) a packet waiting to be read from
      . chip-memory.
      .
      . o Read the status
      . o If an error, record it
      . o otherwise, read in the packet
      --------------------------------------------------------------
    */
    unsigned char *data;
    unsigned short pnr, plen;
    NET_netbuf_t *packet = NULL;

    /*
     * assume bank 2 
     */
    SMC_SELECT_BANK (2);

    if ((pnr = USO_in_w (smc->io_addr + SMC_FIFO)) & SMC_FIFO_REMPTY)
    {
        /*
         * we got called , but nothing was on the FIFO 
         */
        /*
         * don't need to restore anything 
         */
        DEBUGF (DEV_SMC_DEBUG, ("Smc: no p on rx fifo.\n"));
        return packet;
    }

    /*
     * start reading from the start of the packet 
     */
    USO_out_w (smc->io_addr + SMC_PTR, SMC_PTR_READ | SMC_PTR_RCV | SMC_PTR_AUTOINCR);

    /*
     * First two words are status and packet_length 
     */
    pnr = USO_in_w (smc->io_addr + SMC_DATA1);
    plen = (USO_in_w (smc->io_addr + SMC_DATA1) & 0x07ff) - 6;

    if (plen > 10)
    {                           // size of eth header
        if (pnr & SMC_RFS_ERRORS)
        {
            ++smc->rx_error;
            DEBUGF (DEV_SMC_DEBUG, ("Smc: smc rx error.\n"));
        }
        else
        {
            if (pnr & SMC_RFS_ODDFRAME)
                ++plen;
            if ((packet = NET_netbuf_alloc_ram (plen)) != NULL)
            {
                data = (unsigned char *)NET_netbuf_index (packet);
                USO_in_w_stream (smc->io_addr + SMC_DATA1, (unsigned long)data, ((plen + 1) >> 1));
                if (plen & 1)
                {
                    data += plen & ~1;
                    *(data++) = USO_in_b (smc->io_addr + SMC_DATA1);
                }
            }
            else
            {
                DEBUGF (DEV_SMC_DEBUG, ("Smc: alloc error.\n"));
                ++smc->rx_dropped;
            }
        }
    }
    else
    {
        DEBUGF (DEV_SMC_DEBUG, ("Smc: wrong p len.\n"));
        ++smc->rx_badlen;
    }
    /*
     * error or good, tell the card to get rid of this packet 
     */
    USO_out_w (smc->io_addr + SMC_MMUCR, SMC_MMUCR_RELEASE);
    ++smc->rx_packets;
    return packet;
}

static NET_netbuf_t *
smc_receive_packet (DEV_smc91c94_t * smc)
{
    USO_wait (&smc->rx_sem);
    NET_netbuf_t *packet = smc_rx_read_data (smc);
    smc_enable_int (SMC_IM_RCV_INT, smc);
    return packet;
}




/*------------------------------------------------------------------------
 *  ethwrite - write a single packet to the ethernet
 *------------------------------------------------------------------------
 */

static ACE_bool_t
smc_tx_alloc_mem (NET_netbuf_t * packet, DEV_smc91c94_t * smc)
{
    unsigned short numPages;

    /*
     * . the MMU wants the number of pages to be the number of 256 bytes . 
     * 'pages', minus 1 ( since a packet can't ever have 0 pages :) ) 
     */
    numPages = NET_netbuf_tot_len (packet) / 256;

    if (numPages > 7)
    {
        /*
         * freeing the packet is a good thing here... but should . any
         * packets of this size get down here? 
         */
        // debug2(("packet=%p len=%d to big!!!", packet, packet->len));
        return (FALSE);
    }

    ETHIRQ_OFF ();

    /*
     * now, try to allocate the memory 
     */
    SMC_SELECT_BANK (2);
    USO_out_w (smc->io_addr + SMC_MMUCR, SMC_MMUCR_ALLOC | numPages);

    ETHIRQ_ON ();
    return (TRUE);
}


/*
 * . Purpose: .  This sends the actual packet to the SMC9xxx chip. . .
 * Algorithm: .  First, see if a saved_skb is available. .  ( this should
 * NOT be called if there is no 'saved_skb' .  Now, find the packet number 
 * that the chip allocated .  Point the data pointers at it in memory .
 * Set the length word in the chip's memory .  Dump the packet to chip
 * memory .  Check if a last BYTE is needed ( odd length packet ) .  if
 * so, set the control flag right .  Tell the card to send it .  Enable
 * the transmit interrupt, so I know if it failed .  Free the kernel data
 * if I actually sent it. 
 */
static unsigned char
smc_tx_write_data (DEV_smc91c94_t * smc, NET_netbuf_t * packet, ACE_bool_t wirq)
{
    unsigned char packet_no;
    long p_len;
    p_len = NET_netbuf_tot_len (packet);

    if (p_len < 64)
        p_len = 64;
    if (p_len & 1)
        p_len++;
    /*
     * If I get here, I _know_ there is a packet slot waiting for me 
     */
    packet_no = USO_in_b (smc->io_addr + SMC_ARR);

    if (packet_no & 0x80)
    {
        /*
         * or isn't there? BAD CHIP! 
         */
        ++smc->tx_allocfailed;
        return (FALSE);
    }


    /*
     * we have a packet address, so tell the card to use it 
     */
    USO_out_b (smc->io_addr + SMC_PNR, packet_no);

    /*
     * point to the beginning of the packet 
     */
    USO_out_w (smc->io_addr + SMC_PTR, SMC_PTR_AUTOINCR);

    // memcpy(packet->ep_hdr.e_src, etpaddr, sizeof(etpaddr));

    /*
     * send the packet length ( +6 for status, length and ctl BYTE ) and
     * the status word ( set to zeros ) 
     */
    USO_out_w (smc->io_addr + SMC_DATA1, 0);
    /*
     * send the packet length ( +6 for status words, length, and ctl 
     */
    USO_out_w (smc->io_addr + SMC_DATA1, p_len + 6);

    /*
     * send the actual data . I _think_ it's faster to send the longs
     * first, and then . mop up by sending the last word.  It depends
     * heavily . on alignment, at least on the 486.  Maybe it would be . a 
     * good idea to check which is optimal? But that could take . almost
     * as much time as is saved? 
     */
    for (; packet != NULL; packet = NET_netbuf_next (packet))
    {
        USO_out_w_stream (smc->io_addr + SMC_DATA1,
                    (unsigned long)NET_netbuf_index (packet), NET_netbuf_len (packet) >> 1);

        /*
         * Send the last BYTE, if there is one.  
         */

        /*
         * !!! think about the odd byte, work not finished !!!
         */

        if ((NET_netbuf_len (packet) & 1) == 0)
        {
            // SMC_out_w(0, smc->io_addr + SMC_DATA1);
        }
        else
        {
            USO_out_b (smc->io_addr + SMC_DATA1,
                       *((unsigned char *)NET_netbuf_index (packet) + NET_netbuf_len (packet) - 1));
            USO_out_b (smc->io_addr + SMC_DATA1, 0x00);
        }
    }

    /*
     * enable the interrupts 
     */
    if (wirq == TRUE)
    {
        smc_enable_int ((SMC_IAM_TX_INT | SMC_IAM_TX_EMPTY_INT), smc);
    }
    else
    {
        ++smc->tx_packets;
    }

    /*
     * and let the chipset deal with it 
     */
    USO_out_w (smc->io_addr + SMC_MMUCR, SMC_MMUCR_ENQUEUE);

    return (TRUE);
}

static void
smc_transmit_packet (DEV_smc91c94_t * smc, NET_netbuf_t * packet)
{
    unsigned char status;

    if (smc_tx_alloc_mem (packet, smc) == TRUE)
    {
        SMC_SELECT_BANK (2);
        for (;;)
        {
            status = USO_in_b (smc->io_addr + SMC_IST);
            if (status & SMC_IM_ALLOC_INT)
                break;
        }
        smc_tx_write_data (smc, packet, FALSE);
    }
    NET_netbuf_free (packet);
}

static void
smc_info (DEV_smc91c94_t * smc)
{
    ACE_printf ("\tSMC91C94 io_addr: %0lx\n"
                "\tRX: %lu, ovr %u, drop %u, err %u, bad %u\n"
                "\tTX: %lu, err %u, empty %u, alloc fail %u\n"
                "\tINT: early rx %u, eph %u, alloc %u\n"
                "\tLINK: %s, down cnt %u\n",
                smc->io_addr,
                smc->rx_packets,
                smc->rx_overruns,
                smc->rx_dropped,
                smc->rx_error,
                smc->rx_badlen,
                smc->tx_packets,
                smc->tx_errors,
                smc->tx_empty,
                smc->tx_allocfailed,
                smc->ercv_ints,
                smc->eph_ints,
                smc->alloc_ints, smc->link_is_up ? "up" : "down", smc->link_down_count);
}

extern void
DEV_smc_init (NET_ethif_t * ethif, DEV_smc91c94_t * smc, unsigned long io_addr)
{
    ethif->mac = smc;
    ethif->start = (void (*)(void *))smc_start;
    ethif->receive = (NET_netbuf_t * (*)(void *))smc_receive_packet;
    ethif->transmit = (void (*)(void *, NET_netbuf_t *))smc_transmit_packet;
    ethif->info = (void (*)(void *))smc_info;
    smc->io_addr = io_addr;
    memcpy (smc->mac_addr, ethif->eth_addr->addr, NET_ETH_ADDR_SIZE);
    smc->rx_packets = 0;
    smc->rx_overruns = 0;
    smc->rx_dropped = 0;
    smc->rx_error = 0;
    smc->rx_badlen = 0;
    smc->tx_packets = 0;
    smc->tx_errors = 0;
    smc->tx_empty = 0;
    smc->tx_allocfailed = 0;
    smc->ercv_ints = 0;
    smc->eph_ints = 0;
    smc->alloc_ints = 0;
    smc->tx_exc_def = 0;
    smc->tx_def = 0;
    smc->tx_mcc = 0;
    smc->tx_scc = 0;
    smc->link_down_count = 0;
    smc->link_is_up = FALSE;
    USO_semaphore_init (&smc->rx_sem, 0);
}
