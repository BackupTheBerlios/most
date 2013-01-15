/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include "net/debug.h"
#include "net/netif.h"


NET_netif_t *NET_netif_default = NULL;

void
NET_netif_set_ipaddr (NET_netif_t * netif, NET_ip_addr_t * ipaddr)
{
    NET_ip_addr_set (&(netif->ip_addr), ipaddr);
}

void
NET_netif_set_gateway (NET_netif_t * netif, NET_ip_addr_t * gateway)
{
    NET_ip_addr_set (&(netif->gateway), gateway);
}

void
NET_netif_set_netmask (NET_netif_t * netif, NET_ip_addr_t * netmask)
{
    NET_ip_addr_set (&(netif->netmask), netmask);
}

void
NET_netif_set_default (NET_netif_t * netif)
{
    NET_netif_default = netif;
}

static void
print_ipaddr (char *name, NET_ip_addr_t ipaddr)
{
    long addr = ACE_ntohl (ipaddr.addr);
    ACE_printf ("\n\t%s: %3ld.%3ld.%3ld.%3ld",
                name, addr >> 24 & 0xff, addr >> 16 & 0xff, addr >> 8 & 0xff, addr & 0xff);
}

static void
info (MFS_descriptor_t * desc)
{
    NET_netif_t *netif = (NET_netif_t *) desc->represent;

    print_ipaddr ("ip_addr", netif->ip_addr);
    print_ipaddr ("netmask", netif->netmask);
    print_ipaddr ("gateway", netif->gateway);
    ACE_printf ("\n\tTX: %lu drop %u RX: %lu drop %u\n",
                netif->tx, netif->tx_drop, netif->rx, netif->rx_drop);
}

static struct MFS_descriptor_op netif_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
};

extern void
NET_netif_init (NET_netif_t * netif, char *name)
{
    netif->tx = 0;
    netif->rx = 0;
    netif->tx_drop = 0;
    netif->rx_drop = 0;
    netif->device = NULL;
    NET_ip_addr_set (&(netif->ip_addr), &NET_ip_addr_any);
    NET_ip_addr_set (&(netif->netmask), &NET_ip_addr_any);
    NET_ip_addr_set (&(netif->gateway), &NET_ip_addr_any);
    MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/net/netif"),
                     name, MFS_SYS, &netif_descriptor_op, (MFS_represent_t *) netif);
}
