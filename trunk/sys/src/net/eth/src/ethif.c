/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include "net/debug.h"
#include "net/ethif.h"
#include "net/arp.h"
#include "net/arp.h"
#include "net/ethernet.h"
#include "net/ip.h"

static void
ethif_run (void *netif)
{
    USO_log_puts (USO_LL_INFO, "Ethif is running.\n");
    NET_ethif_t *ethif = ((NET_netif_t *) netif)->device;
    for (;;)
    {
        if (ethif->receive == NULL)
        {
            break;
        }
        NET_netbuf_t *p = ethif->receive (ethif->mac);
        if (p != NULL)
        {
            NET_eth_input ((NET_netif_t *) netif, p);
        }
    }
}

static void
info (MFS_descriptor_t * desc)
{
    NET_ethif_t *ethif = (NET_ethif_t *) desc->represent;

    ACE_printf ("\teth_addr: %02X %02X %02X %02X %02X %02X\n",
                ethif->eth_addr->addr[0],
                ethif->eth_addr->addr[1],
                ethif->eth_addr->addr[2],
                ethif->eth_addr->addr[3], ethif->eth_addr->addr[4], ethif->eth_addr->addr[5]);
    if (ethif->info != NULL)
    {
        ethif->info (ethif->mac);
    }
}

static struct MFS_descriptor_op ethif_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
};

extern void
NET_ethif_init (NET_netif_t * netif, NET_ethif_t * ethif, struct NET_eth_addr *addr, char *name)
{
    netif->device = ethif;
    netif->output = NET_eth_output;
    ethif->eth_addr = addr;
    ethif->mac = NULL;
    ethif->start = NULL;
    ethif->receive = NULL;
    ethif->transmit = NULL;
    ethif->info = NULL;
    USO_thread_init (&ethif->rx_thread,
                     ethif_run,
                     ethif->rx_stack, ACE_ARRAYSIZE (ethif->rx_stack),
                     USO_INTERRUPT, USO_FIFO, name);
    USO_thread_arg_init (&ethif->rx_thread, netif);
    MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/net/netif"),
                     name, MFS_SYS, &ethif_descriptor_op, (MFS_represent_t *) ethif);
}

extern void
NET_ethif_start (NET_ethif_t * ethif)
{
    USO_start (&ethif->rx_thread);
    if (ethif->start != NULL)
    {
        ethif->start (ethif->mac);
    }
}
