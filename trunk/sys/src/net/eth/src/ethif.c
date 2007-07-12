/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/sleep.h"
#include "net/debug.h"
#include "net/ethif.h"
#include "net/arp.h"
#include "net/stats.h"
#include "net/ethdev.h"

static const struct NET_eth_addr ethbroadcast =
    { {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} };

static NET_err_t
ethif_output (NET_netif_t * netif, NET_netbuf_t * p, NET_ip_addr_t * ipaddr)
{
    NET_ethif_t *ethif;
    NET_netbuf_t *q;
    struct NET_eth_hdr *ethhdr;
    struct NET_eth_addr *dest, mcastaddr;
    NET_ip_addr_t *queryaddr;
    u8_t i;

    ethif = netif->state;

    NET_netbuf_index_inc (p, -sizeof (struct NET_eth_hdr));

    /*
     * Construct Ethernet header. Start with looking up deciding which MAC 
     * address to use as a destination address. Broadcasts and multicasts
     * are special, all other addresses are looked up in the ARP table. 
     */
    queryaddr = ipaddr;
    if (NET_ip_addr_isany (ipaddr) ||
        NET_ip_addr_isbroadcast (ipaddr, &(netif->netmask)))
    {
        dest = (struct NET_eth_addr *)&ethbroadcast;
    }
    else if (NET_ip_addr_ismulticast (ipaddr))
    {
        /*
         * Hash IP multicast address to MAC address. 
         */
        mcastaddr.addr[0] = 0x01;
        mcastaddr.addr[1] = 0x0;
        mcastaddr.addr[2] = 0x5e;
        mcastaddr.addr[3] = NET_ip4_addr2 (ipaddr) & 0x7f;
        mcastaddr.addr[4] = NET_ip4_addr3 (ipaddr);
        mcastaddr.addr[5] = NET_ip4_addr4 (ipaddr);
        dest = &mcastaddr;
    }
    else
    {

        if (NET_ip_addr_maskcmp (ipaddr, &(netif->ip_addr), &(netif->netmask)))
        {
            /*
             * Use destination IP address if the destination is on the
             * same subnet as we are. 
             */
            queryaddr = ipaddr;
        }
        else
        {
            /*
             * Otherwise we use the default router as the address to send
             * the Ethernet frame to. 
             */
            queryaddr = &(netif->gateway);
        }
        dest = NET_arp_lookup (queryaddr);
    }

    /*
     * If the arp_lookup() didn't find an address, we send out an ARP
     * query for the IP address. 
     */
    int arp_request_count = 3;
    while (dest == NULL)
    {
        q = NET_arp_query (netif, ethif->ethaddr, queryaddr);
        if (q != NULL)
        {
            USO_post (&ethif->ll_tx_que, (USO_node_t *) q);
            if(arp_request_count--){
            	USO_sleep(ACE_MSEC_2_TICKS(100));
		        dest = NET_arp_lookup (queryaddr);
            }else {
            	return NET_ERR_ARP;
            }
        } else {
        	return NET_ERR_MEM;
        }
    }
    ethhdr = (struct NET_eth_hdr *)p->index;

    for (i = 0; i < 6; i++)
    {
        ethhdr->dest.addr[i] = dest->addr[i];
        ethhdr->src.addr[i] = ethif->ethaddr->addr[i];
    }

    ethhdr->type = htons (NET_ETH_TYPE_IP);

    USO_post (&ethif->ll_tx_que, (USO_node_t *) p);

    return NET_ERR_OK;
}

static void
ethif_input (NET_netif_t * netif, NET_netbuf_t * p)
{
    struct NET_ethif *ethif;
    struct NET_eth_hdr *ethhdr;

    ethif = netif->state;

    ethhdr = (struct NET_eth_hdr *)p->data;

    switch (htons (ethhdr->type))
    {
    case NET_ETH_TYPE_IP:
        DEBUGF (NET_ETH_DEBUG, ("Eth: rx ip.\n"));

        NET_arp_ip_input (netif, p);
        NET_netbuf_index_inc (p, sizeof (struct NET_eth_hdr));
        if (netif->input (netif, p) < NET_ERR_OK)
        {
            NET_netbuf_free (p);
        }
        break;
    case NET_ETH_TYPE_ARP:
        USO_kputs (USO_LL_INFO, "Eth: rx arp.\n");
        p = NET_arp_arp_input (netif, ethif->ethaddr, p);
        if (p != NULL)
        {
            USO_post (&ethif->ll_tx_que, (USO_node_t *) p);
        }
        break;
    default:
        USO_kputs (USO_LL_INFO, "Eth: rx unsuport.\n");
        NET_netbuf_free (p);
        break;
    }
}

static void
eth_ll_rx_enter (void *netif)
{
    NET_ethif_t *ethif = ((NET_netif_t *) netif)->state;
    NET_ethdev_t *ethdev = ethif->ethdev;
    for (;;)
    {
        NET_netbuf_t *p = ethdev->receive_packet (ethdev, &ethif->rx_pool);
        if (p != NULL)
        {
            ethif_input ((NET_netif_t *) netif, p);
        }
    }
}

static void
eth_ll_tx_enter (void *ethif)
{
    NET_ethdev_t *ethdev = ((NET_ethif_t *) ethif)->ethdev;
    for (;;)
    {
        NET_netbuf_t *p =
            (NET_netbuf_t *) USO_fetch (&((NET_ethif_t *) ethif)->ll_tx_que);
        ethdev->transmit_packet (ethdev, p);
        NET_netbuf_free (p);
    }
}

extern void
NET_ethif_start (NET_netif_t * netif)
{
    NET_ethif_t *ethif = netif->state;
    USO_thread_init (&ethif->ll_tx_thread,
                     eth_ll_tx_enter,
                     ethif->ll_tx_stack, ARRAYSIZE (ethif->ll_tx_stack),
                     USO_SYSTEM, USO_FIFO, "Eth_tx");
    USO_thread_arg_init(&ethif->ll_tx_thread, ethif, FALSE);
    USO_start (&ethif->ll_tx_thread);

    USO_thread_init (&ethif->ll_rx_thread,
                     eth_ll_rx_enter,
                     ethif->ll_rx_stack, ARRAYSIZE (ethif->ll_rx_stack),
                     USO_SYSTEM, USO_FIFO, "Eth_rx");
    USO_thread_arg_init(&ethif->ll_rx_thread, netif, FALSE);
    USO_start (&ethif->ll_rx_thread);
}


extern void
NET_ethif_init (NET_ethif_t * ethif,
                NET_netif_t * netif,
                struct NET_eth_addr *hwaddr, NET_ethdev_t * ethdev)
{
    netif->state = ethif;
    netif->output = ethif_output;

    NET_arp_init ();

    ethif->ethaddr = hwaddr;
    ethif->ethdev = ethdev;

    USO_mailbox_init (&ethif->ll_tx_que, NET_ETH_RX_TX_QUE_SIZE);

    USO_buf_pool_init (&ethif->rx_pool, ethif->rx_packets,
                       NET_ETH_RX_TX_QUE_SIZE, sizeof (NET_netbuf_t));
}
