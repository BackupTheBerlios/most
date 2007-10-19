/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/sleep.h>

#include "net/debug.h"
#include "net/ethif.h"
#include "net/arp.h"
#include "net/ethernet.h"
#include "net/stats.h"
#include "net/ip.h"

static const struct NET_eth_addr ethbroadcast =
    { {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} };

extern void
NET_eth_input (NET_netif_t * netif, NET_netbuf_t * p)
{
    struct NET_ethif *ethif;
    struct NET_eth_hdr *ethhdr;

    ethif = netif->device;

    ethhdr = (struct NET_eth_hdr *)p->data;

    switch (htons (ethhdr->type))
    {
    case NET_ETH_TYPE_IP:
        DEBUGF (NET_ETH_DEBUG, ("Eth: rx ip.\n"));

        NET_arp_ip_input (netif, p);
        NET_netbuf_index_inc (p, sizeof (struct NET_eth_hdr));
        if (NET_ip_input (netif, p) < NET_ERR_OK)
        {
            NET_netbuf_free (p);
        }
        break;
    case NET_ETH_TYPE_ARP:
        USO_kputs (USO_LL_INFO, "Eth: rx arp.\n");
        p = NET_arp_arp_input (netif, ethif->eth_addr, p);
        if (p != NULL)
        {
            if (ethif->transmit != NULL) {ethif->transmit(ethif->mac, p);}
        }
        break;
    default:
        USO_kputs (USO_LL_INFO, "Eth: rx unsuport.\n");
        NET_netbuf_free (p);
        break;
    }
}

extern NET_err_t
NET_eth_output (NET_netif_t * netif, NET_netbuf_t * p, NET_ip_addr_t * ipaddr)
{
    NET_ethif_t *ethif;
    NET_netbuf_t *q;
    struct NET_eth_hdr *ethhdr;
    struct NET_eth_addr *dest, mcastaddr;
    NET_ip_addr_t *queryaddr;
    u8_t i;

    ethif = netif->device;
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
        q = NET_arp_query (netif, ethif->eth_addr, queryaddr);
        if (q != NULL)
        {
            if (ethif->transmit != NULL) { ethif->transmit(ethif->mac, q); }
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
        ethhdr->src.addr[i] = ethif->eth_addr->addr[i];
    }
    ethhdr->type = htons (NET_ETH_TYPE_IP);
    if (ethif->transmit != NULL) { ethif->transmit (ethif->mac, p); }
    return NET_ERR_OK;
}

extern void
NET_eth_init (void)
{
    NET_arp_init ();
}
