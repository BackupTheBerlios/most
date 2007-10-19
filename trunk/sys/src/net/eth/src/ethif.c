/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "net/debug.h"
#include "net/ethif.h"
#include "net/arp.h"
#include "net/arp.h"
#include "net/ethernet.h"
#include "net/ip.h"

static void
ethif_run (void *netif)
{
	NET_ethif_t *ethif = ((NET_netif_t *)netif)->device;
    for (;;)
    {
    	if (ethif->receive == NULL) {break;}
        NET_netbuf_t *p = ethif->receive (ethif->mac);
        if (p != NULL)
        {
            NET_eth_input ((NET_netif_t *) netif, p);
        }
    }
}

extern void
NET_ethif_init (NET_netif_t *netif,
				NET_ethif_t *ethif,
                struct NET_eth_addr *eth_addr)
{
    netif->device = ethif;
    netif->output = NET_eth_output;
    ethif->eth_addr = eth_addr;
    ethif->mac = NULL;
    ethif->start = NULL;
    ethif->receive = NULL;
    ethif->transmit = NULL;
    USO_thread_init (&ethif->rx_thread,
                     ethif_run,
                     ethif->rx_stack, ARRAYSIZE (ethif->rx_stack),
                     USO_INTERRUPT, USO_FIFO, "ethif");
    USO_thread_arg_init(&ethif->rx_thread, netif, FALSE);
}

extern void
NET_ethif_start (NET_ethif_t * ethif)
{
    USO_start (&ethif->rx_thread);
	if (ethif->start != NULL) { ethif->start(ethif->mac); }
}
