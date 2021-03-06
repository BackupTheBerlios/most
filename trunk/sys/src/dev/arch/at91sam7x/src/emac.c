#include <ace/stddef.h>
#include <ace/string.h>
#include <uso/sleep.h>
#include <uso/semaphore.h>
#include <uso/mutex.h>
#include <dev/arch/at91sam7x/AT91SAM7X256.h>
#include <dev/arch/at91sam7x/aic.h>
#include "dev/arch/at91sam7x/emac.h"
#include "dev/arch/at91sam7x/emac_def.h"
#include "dev/mii.h"
#include "dev/debug.h"
#include "net/netif.h"
#include "net/ethif.h"

/* The buffer addresses written into the descriptors must be aligned so the
last few bits are zero.  These bits have special meaning for the EMAC
peripheral and cannot be used as part of the address. */
#define AT91_EMAC_ADDRESS_MASK			( ( unsigned long ) 0xFFFFFFFC )

/* Bit used within the address stored in the descriptor to mark the last
descriptor in the array. */
#define AT91_EMAC_RX_WRAP_BIT				( ( unsigned long ) 0x02 )

/* Bit used within the Tx descriptor status to indicate whether the
descriptor is under the control of the EMAC or the software. */
#define AT91_EMAC_TX_BUF_USED				( ( unsigned long ) 0x80000000 )

/* A short delay is used to wait for a buffer to become available, should
one not be immediately available when trying to transmit a frame. */
#define AT91_EMAC_BUFFER_WAIT_DELAY 		( USO_MSEC_2_TICKS(1) )
#define AT91_EMAC_MAX_WAIT_CYCLES			( 10 )

/* Misc defines. */
#define AT91_EMAC_INTERRUPT_LEVEL			( 5 )
#define AT91_EMAC_NO_DELAY   				( 0 )
#define AT91_EMAC_TOTAL_FRAME_HEADER_SIZE	( 54 )
#define AT91_EMAC_PHY_INIT_DELAY			( USO_MSEC_2_TICKS(2000) )
#define AT91_EMAC_RESET_KEY				( ( unsigned long ) 0xA5000000 )
#define AT91_EMAC_RESET_LENGTH			( ( unsigned long ) ( 0x01 << 8 ) )

static void
AT91F_PIO_CfgPeriph (AT91PS_PIO pPio,   // \arg pointer to a PIO controller
                     unsigned int periphAEnable,        // \arg PERIPH A to enable
                     unsigned int periphBEnable)        // \arg PERIPH B to enable
{
    pPio->PIO_ASR = periphAEnable;
    pPio->PIO_BSR = periphBEnable;
    pPio->PIO_PDR = (periphAEnable | periphBEnable);    // Set in Periph mode
}

static void
AT91F_EMAC_CfgPIO (void)
{
    // Configure PIO controllers to periph mode
    AT91F_PIO_CfgPeriph (AT91C_BASE_PIOB,       // PIO controller base address
                         ((unsigned int)AT91C_PB2_ETX0) |
                         ((unsigned int)AT91C_PB12_ETXER) |
                         ((unsigned int)AT91C_PB16_ECOL) |
                         ((unsigned int)AT91C_PB11_ETX3) | ((unsigned int)AT91C_PB6_ERX1) |
                         //(unsigned int) AT91C_PB15_ERXDV   ) |
                         ((unsigned int)AT91C_PB15_ERXDV_ECRSDV) |
                         ((unsigned int)AT91C_PB13_ERX2) |
                         ((unsigned int)AT91C_PB3_ETX1) |
                         ((unsigned int)AT91C_PB8_EMDC) | ((unsigned int)AT91C_PB5_ERX0) |
                         //((unsigned int) AT91C_PB18_EF100   ) |
                         ((unsigned int)AT91C_PB14_ERX3) |
                         //((unsigned int) AT91C_PB4_ECRS_ECRSDV) |
                         ((unsigned int)AT91C_PB4_ECRS) | ((unsigned int)AT91C_PB1_ETXEN) | ((unsigned int)AT91C_PB10_ETX2) | ((unsigned int)AT91C_PB0_ETXCK_EREFCK) | ((unsigned int)AT91C_PB9_EMDIO) | ((unsigned int)AT91C_PB7_ERXER) | ((unsigned int)AT91C_PB17_ERXCK),    // Peripheral A
                         0);    // Peripheral B
}

/* Buffer written to by the EMAC DMA.  Must be aligned as described by the
comment above the emacADDRESS_MASK definition. */
static volatile char rx_buffer[NB_RX_BUFFERS * ETH_RX_BUFFER_SIZE] ACE_ALIGNED_ (8);

/* Buffer read by the EMAC DMA.  Must be aligned as described by he comment
above the emacADDRESS_MASK definition. */
static char tx_buffer[NB_TX_BUFFERS * ETH_TX_BUFFER_SIZE] ACE_ALIGNED_ (8);

/* Descriptors used to communicate between the program and the EMAC peripheral.
These descriptors hold the locations and state of the Rx and Tx buffers. */
static volatile tx_descriptor_t tx_descriptors[NB_TX_BUFFERS];
static volatile rx_descriptor_t rx_descriptors[NB_RX_BUFFERS];

static void
setup_descriptors (void)
{
    int i;
    unsigned long address;

    for (i = 0; i < NB_RX_BUFFERS; ++i)
    {
        address = (unsigned long)(rx_buffer + (i * ETH_RX_BUFFER_SIZE));
        rx_descriptors[i].addr = address & AT91_EMAC_ADDRESS_MASK;
    }

    rx_descriptors[NB_RX_BUFFERS - 1].addr |= AT91_EMAC_RX_WRAP_BIT;

    for (i = 0; i < NB_TX_BUFFERS; ++i)
    {
        address = (unsigned long)(tx_buffer + (i * ETH_TX_BUFFER_SIZE));
        tx_descriptors[i].addr = address & AT91_EMAC_ADDRESS_MASK;
        tx_descriptors[i].U_Status.status = AT91C_TRANSMIT_OK;
    }

    tx_descriptors[NB_TX_BUFFERS - 1].U_Status.status = AT91C_TRANSMIT_WRAP | AT91C_TRANSMIT_OK;

    AT91C_BASE_EMAC->EMAC_RBQP = (unsigned long)rx_descriptors;
    AT91C_BASE_EMAC->EMAC_TBQP = (unsigned long)tx_descriptors;

    AT91C_BASE_EMAC->EMAC_RSR = (AT91C_EMAC_OVR | AT91C_EMAC_REC | AT91C_EMAC_BNA);
//      AT91C_BASE_EMAC->EMAC_NCFGR |= ( AT91C_EMAC_CAF | AT91C_EMAC_NBC | AT91C_EMAC_DRFCS);
    AT91C_BASE_EMAC->EMAC_NCFGR |= (AT91C_EMAC_MTI | AT91C_EMAC_DRFCS);
    AT91C_BASE_EMAC->EMAC_NCR |= (AT91C_EMAC_TE | AT91C_EMAC_RE | AT91C_EMAC_WESTAT);
}

static void
setup_mac_address (DEV_at91_emac_t * mac)
{
    /* Must be written SA1L then SA1H. */
    AT91C_BASE_EMAC->EMAC_SA1L = ((unsigned long)mac->mac_addr[3] << 24) |
        ((unsigned long)mac->mac_addr[2] << 16) |
        ((unsigned long)mac->mac_addr[1] << 8) | (mac->mac_addr[0]);

    AT91C_BASE_EMAC->EMAC_SA1H = ((unsigned long)mac->mac_addr[5] << 8) | (mac->mac_addr[4]);
}

static void
setup_hash_address (void)
{
    /* Must be written HRB then HRT. */
    AT91C_BASE_EMAC->EMAC_HRB = 0xFFFFFFFF;

    AT91C_BASE_EMAC->EMAC_HRT = 0xFFFFFFFF;
}

static void
read_phy (unsigned char phy_address, unsigned char address, unsigned long *value)
{
    AT91C_BASE_EMAC->EMAC_MAN = (AT91C_EMAC_SOF & (0x01 << 30))
        | (2 << 16) | (2 << 28) | ((phy_address & 0x1f) << 23) | (address << 18);
    while (!(AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE));
    *value = (AT91C_BASE_EMAC->EMAC_MAN & 0x0000ffff);
    DEBUGF (DEV_MAC_DEBUG, ("Mac: read %lx.\n", *value));
}

static void
write_phy (unsigned char phy_address, unsigned char address, unsigned long value)
{
    AT91C_BASE_EMAC->EMAC_MAN = ((AT91C_EMAC_SOF & (0x01 << 30))
                                 | (2 << 16) | (1 << 28)
                                 | ((phy_address & 0x1f) << 23)
                                 | (address << 18)) | (value & 0xffff);

    while (!(AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE));
}

static int
get_link_speed (void)
{
    unsigned long bmsr, bmcr, lpa, mac_cfg, speed, duplex;

    /* Link status is latched, so read twice to get current value */
    read_phy (AT91C_PHY_ADDR, MII_BMSR, &bmsr);
    read_phy (AT91C_PHY_ADDR, MII_BMSR, &bmsr);

    if (!(bmsr & BMSR_LSTATUS))
    {
        DEBUGF (DEV_MAC_DEBUG, ("Mac: no link.\n"));
        return -1;
    }

    read_phy (AT91C_PHY_ADDR, MII_BMCR, &bmcr);
    if (bmcr & BMCR_ANENABLE)
    {
        /* AutoNegotiation is enabled. */
        if (!(bmsr & BMSR_ANEGCOMPLETE))
        {
            DEBUGF (DEV_MAC_DEBUG, ("Mac: auto negotiation in progress.\n"));
            return -1;
        }

        read_phy (AT91C_PHY_ADDR, MII_LPA, &lpa);
        if ((lpa & LPA_100FULL) || (lpa & LPA_100HALF))
        {
            speed = SPEED_100;
        }
        else
        {
            speed = SPEED_10;
        }

        if ((lpa & LPA_100FULL) || (lpa & LPA_10FULL))
        {
            duplex = DUPLEX_FULL;
        }
        else
        {
            duplex = DUPLEX_HALF;
        }

    }
    else
    {
        speed = (bmcr & BMCR_SPEED100) ? SPEED_100 : SPEED_10;
        duplex = (bmcr & BMCR_FULLDPLX) ? DUPLEX_FULL : DUPLEX_HALF;
    }

    DEBUGF (DEV_MAC_DEBUG, ("Mac: Speed: %s, Duplex: %s.\n",
                            speed == SPEED_100 ? "100" : "10",
                            duplex == DUPLEX_FULL ? "full" : "half"));

    mac_cfg = AT91C_BASE_EMAC->EMAC_NCFGR & ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
    if (speed == SPEED_100)
    {
        if (duplex == DUPLEX_FULL)
        {
            AT91C_BASE_EMAC->EMAC_NCFGR = mac_cfg | AT91C_EMAC_SPD | AT91C_EMAC_FD;
        }
        else
        {
            AT91C_BASE_EMAC->EMAC_NCFGR = mac_cfg | AT91C_EMAC_SPD;
        }
    }
    else
    {
        if (duplex == DUPLEX_FULL)
        {
            AT91C_BASE_EMAC->EMAC_NCFGR = mac_cfg | AT91C_EMAC_FD;
        }
        else
        {
            AT91C_BASE_EMAC->EMAC_NCFGR = mac_cfg;
        }
    }
    return 0;
}

static int
probe_phy (void)
{
    unsigned long phy_id, phy_id_1, phy_id_2, status;
    int ret = 0;

    /* Enable management port */
    AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;
    AT91C_BASE_EMAC->EMAC_NCFGR |= (2) << 10;

    /* Read the PHY ID. */
    read_phy (AT91C_PHY_ADDR, MII_PHYSID1, &phy_id_1);
    read_phy (AT91C_PHY_ADDR, MII_PHYSID2, &phy_id_2);

    phy_id = (phy_id_1 << 16) | (phy_id_2 & 0xfff0);
    if (phy_id != MII_MICREL_ID)
    {
        DEBUGF (DEV_MAC_DEBUG, ("Mac: phy id %d != Micrel id %d.\n", phy_id, MII_MICREL_ID));
        ret = -1;
    }
    else
    {
        status = get_link_speed ();
        if (status != 0)
        {
            DEBUGF (DEV_MAC_DEBUG, ("Mac: can not get link_speed.\n"));
            ret = -1;
        }
    }

    /* Disable management port */
    AT91C_BASE_EMAC->EMAC_NCR &= ~AT91C_EMAC_MPE;

    return ret;
}

extern void
DEV_at91_emac_interrupt (DEV_at91_emac_t * mac)
{
    volatile unsigned long int_status, rx_status;

    int_status = AT91C_BASE_EMAC->EMAC_ISR;
    rx_status = AT91C_BASE_EMAC->EMAC_RSR;

    /* Clear the interrupt befor context switch. */
    AT91C_BASE_AIC->AIC_EOICR = 0;

    if ((int_status & AT91C_EMAC_RCOMP) || (rx_status & AT91C_EMAC_REC))
    {
        /* A frame has been received, signal to rx task for processing the Rx descriptors. */
        AT91C_BASE_EMAC->EMAC_RSR = AT91C_EMAC_REC;
        USO_signal (&mac->rx_sem);
    }

}

static void
at91_emac_start (DEV_at91_emac_t * mac)
{
    /* After PHY power up, hardware reset. */
    AT91C_BASE_RSTC->RSTC_RMR = AT91_EMAC_RESET_KEY | AT91_EMAC_RESET_LENGTH;
    AT91C_BASE_RSTC->RSTC_RCR = AT91_EMAC_RESET_KEY | AT91C_RSTC_EXTRST;

    /* Wait for hardware reset end. */
    while (!(AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_NRSTL));

    /* EMAC IO init for EMAC-PHY com. */
    AT91F_EMAC_CfgPIO ();

    /* Enable com between EMAC PHY. */

    /* Enable management port. */
    AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;

    /* MDC = MCK/32. */
    AT91C_BASE_EMAC->EMAC_NCFGR |= (2) << 10;

    /* Wait for PHY auto init end (rather crude delay!). */
    USO_sleep (AT91_EMAC_PHY_INIT_DELAY);

    /* PHY configuration. */
    unsigned long control;

    /* PHY has internal pull down : disable MII isolate. */
    read_phy (AT91C_PHY_ADDR, MII_BMCR, &control);
    read_phy (AT91C_PHY_ADDR, MII_BMCR, &control);
    control &= ~BMCR_ISOLATE;
    write_phy (AT91C_PHY_ADDR, MII_BMCR, control);

    /* Disable management port again. */
    AT91C_BASE_EMAC->EMAC_NCR &= ~AT91C_EMAC_MPE;

    /* Enable EMAC in MII mode, enable clock ERXCK and ETXCK. */
    AT91C_BASE_EMAC->EMAC_USRIO = AT91C_EMAC_CLKEN;

    setup_descriptors ();

    setup_mac_address (mac);
    setup_hash_address ();

    if (probe_phy () == 0)
    {
        AT91C_BASE_EMAC->EMAC_IER = AT91C_EMAC_RCOMP;
        DEBUGF (DEV_MAC_DEBUG, ("Mac: phy probe done.\n"));
    }
    else
    {
        DEBUGF (DEV_MAC_DEBUG, ("Mac: phy probe failed.\n"));
    }
}


static void
at91_emac_send (DEV_at91_emac_t * mac, NET_netbuf_t * packet)
{
    static unsigned int tx_buffer_index = 0;
    int wait_cycles = 0;
    long ret = 0;
    char *buffer;

    ++mac->tx_packets;

    /* Is a buffer available? */
    while (!(tx_descriptors[tx_buffer_index].U_Status.status & AT91C_TRANSMIT_OK))
    {
        /* There is no room to write the Tx data to the Tx buffer.  Wait a
           short while, then try again. */
        wait_cycles++;
        if (wait_cycles > AT91_EMAC_MAX_WAIT_CYCLES)
        {
            /* Give up. */
            ret = -1;
            break;
        }
        else
        {
            USO_sleep (AT91_EMAC_BUFFER_WAIT_DELAY);
        }
    }

    if (ret == 0)
    {
        NET_netbuf_t *p = packet;
        int packet_len = NET_netbuf_tot_len (p);
        buffer = (char *)tx_descriptors[tx_buffer_index].addr;
        while (p != NULL)
        {
            memcpy ((void *)buffer, NET_netbuf_index (p), NET_netbuf_len (p));
            buffer += NET_netbuf_len (p);
            p = NET_netbuf_next (p);
        }
        /* Send. */

        USO_lock (&mac->lock);
        {
            if (tx_buffer_index >= (NB_TX_BUFFERS - 1))
            {
                /* Fill out the necessary in the descriptor to get the data sent. */
                tx_descriptors[tx_buffer_index].U_Status.status =
                    (packet_len & (unsigned long)AT91C_LENGTH_FRAME)
                    | AT91C_LAST_BUFFER | AT91C_TRANSMIT_WRAP;
                tx_buffer_index = 0;
            }
            else
            {
                /* Fill out the necessary in the descriptor to get the data sent. */
                tx_descriptors[tx_buffer_index].U_Status.status =
                    (packet_len & (unsigned long)AT91C_LENGTH_FRAME) | AT91C_LAST_BUFFER;
                tx_buffer_index++;
            }
            AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TSTART;
        }
        USO_unlock (&mac->lock);
    }
    else
    {
        ++mac->tx_alloc_failed;
    }
    NET_netbuf_free (packet);
}

static NET_netbuf_t *
at91_emac_poll (DEV_at91_emac_t * mac)
{
    NET_netbuf_t *packet = NULL;
    static int next_rx_buffer = 0;
    unsigned long frame_length = 0, section_length = 0, length_so_far = 0;
    unsigned long eof = 0;
    char *source;

    /* Skip any fragments. */
    while ((rx_descriptors[next_rx_buffer].addr & AT91C_OWNERSHIP_BIT) &&
           !(rx_descriptors[next_rx_buffer].U_Status.status & AT91C_SOF))
    {
        /* Mark the buffer as free again. */
        rx_descriptors[next_rx_buffer].addr &= ~(AT91C_OWNERSHIP_BIT);
        next_rx_buffer++;
        if (next_rx_buffer >= NB_RX_BUFFERS)
        {
            next_rx_buffer = 0;
        }
    }

    int i = next_rx_buffer;
    /* Is there a packet ready? */
    while ((rx_descriptors[i].addr & AT91C_OWNERSHIP_BIT) && !frame_length)
    {
        frame_length = rx_descriptors[i].U_Status.status & AT91C_LENGTH_FRAME;

        if (frame_length != 0)
        {
            /* Is this the last buffer for the frame?  If not why? */
            eof = rx_descriptors[i].U_Status.status & AT91C_EOF;
        }

        /* Increment to the next buffer, wrapping if necessary. */
        ++i;
        if (i >= NB_RX_BUFFERS)
        {
            i = 0;
        }
    }

    /* Is there a packet ready? */
    if ((packet = NET_netbuf_alloc_ram (frame_length)) != NULL)
    {
        char *data = NET_netbuf_index (packet);
        while ((rx_descriptors[next_rx_buffer].addr & AT91C_OWNERSHIP_BIT) && !section_length)
        {
            source = (char *)(rx_descriptors[next_rx_buffer].addr & AT91_EMAC_ADDRESS_MASK);
            section_length = rx_descriptors[next_rx_buffer].U_Status.status & AT91C_LENGTH_FRAME;

            if (section_length == 0)
            {
                /* The frame is longer than the buffer pointed to by this
                   descriptor so copy the entire buffer to uIP - then move onto
                   the next descriptor to get the rest of the frame. */
                if ((length_so_far + ETH_RX_BUFFER_SIZE) <= frame_length)
                {
                    memcpy (data, source, ETH_RX_BUFFER_SIZE);
                    length_so_far += ETH_RX_BUFFER_SIZE;
                    data += ETH_RX_BUFFER_SIZE;
                }
            }
            else
            {
                /* This is the last section of the frame. */
                if (section_length <= frame_length)
                {
                    /* The section length holds the length of the entire frame.
                       Length_so_far holds the length of the frame sections already
                       copied. */
                    if (section_length > length_so_far)
                    {
                        memcpy (data, source, (section_length - length_so_far));

                        /* Is this the last buffer for the frame?  If not why? */
                        eof = rx_descriptors[next_rx_buffer].U_Status.status & AT91C_EOF;

                    }
                    else
                    {
                        ++mac->rx_badlen;
                    }
                }
                else
                {
                    ++mac->rx_badlen;
                }

            }

            /* Mark the buffer as free again. */
            rx_descriptors[next_rx_buffer].addr &= ~(AT91C_OWNERSHIP_BIT);

            /* Increment to the next buffer, wrapping if necessary. */
            next_rx_buffer++;
            if (next_rx_buffer >= NB_RX_BUFFERS)
            {
                next_rx_buffer = 0;
            }
        }
        if (!eof)
        {
            ++mac->rx_dropped;
            NET_netbuf_free (packet);
            packet = NULL;
        }
    }
    else
    {
        ++mac->rx_overruns;
    }

    ++mac->rx_packets;
    return packet;
}

static NET_netbuf_t *
at91_emac_recv (DEV_at91_emac_t * mac)
{
    NET_netbuf_t *packet;
    while ((packet = at91_emac_poll (mac)) == NULL)
    {
        USO_wait (&mac->rx_sem);
    }
    return packet;
}

static void
at91_emac_info (DEV_at91_emac_t * mac)
{
    ACE_printf ("\tAT91 EMAC\n"
                "\tRX: %lu, ovr %u, drop %u, bad %u\n"
                "\tTX: %lu, alloc fail %u\n"
                "\tLINK: %s, down cnt %u\n",
                mac->rx_packets,
                mac->rx_overruns,
                mac->rx_dropped,
                mac->rx_badlen,
                mac->tx_packets,
                mac->tx_alloc_failed, mac->link_is_up ? "up" : "down", mac->link_down_count);
}

extern void
DEV_at91_emac_init (NET_ethif_t * ethif, DEV_at91_emac_t * mac)
{
    ethif->mac = mac;
    ethif->start = (void (*)(void *))at91_emac_start;
    ethif->receive = (NET_netbuf_t * (*)(void *))at91_emac_recv;
    ethif->transmit = (void (*)(void *, NET_netbuf_t *))at91_emac_send;
    ethif->info = (void (*)(void *))at91_emac_info;
    memcpy (mac->mac_addr, ethif->eth_addr->addr, NET_ETH_ADDR_SIZE);
    mac->rx_packets = 0;
    mac->rx_overruns = 0;
    mac->rx_dropped = 0;
    mac->rx_badlen = 0;
    mac->tx_packets = 0;
    mac->tx_alloc_failed = 0;
    mac->link_down_count = 0;
    mac->link_is_up = FALSE;
    USO_semaphore_init (&mac->rx_sem, 0);
    USO_mutex_init (&mac->lock);
}
