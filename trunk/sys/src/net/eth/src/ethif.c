/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

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
info (MFS_entry_t * entry)
{
    NET_ethif_t *ethif = (NET_ethif_t *) entry;

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

static struct MFS_descriptor_op ethif_descriptor_op = {.open = NULL,
    .close = NULL,
    .info = info
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
    MFS_create_desc (MFS_sysfs_get_dir (MFS_SYSFS_DIR_NETIF),
                     name, (MFS_entry_t *) ethif, MFS_DESC, &ethif_descriptor_op);
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
