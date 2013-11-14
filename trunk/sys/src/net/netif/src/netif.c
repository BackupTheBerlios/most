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
print_ipaddr (char *buf, NET_ip_addr_t ipaddr)
{
    long addr = ACE_ntohl (ipaddr.addr);
    ACE_sprintf (buf, "%3ld.%3ld.%3ld.%3ld",
                addr >> 24 & 0xff, addr >> 16 & 0xff, addr >> 8 & 0xff, addr & 0xff);
}

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    static char buf[32];
    NET_netif_t *netif = (NET_netif_t *) desc->represent;
    switch (number){
        case 0:
            print_ipaddr (buf, netif->ip_addr);
            entry->type = MFS_INFO_STRING;
            entry->name = "ip addr";
            entry->value.s = buf;
            break;
        case 1:
            print_ipaddr (buf, netif->netmask);
            entry->type = MFS_INFO_STRING;
            entry->name = "netmask";
            entry->value.s = buf;
            break;
        case 2:
            print_ipaddr (buf, netif->gateway);
            entry->type = MFS_INFO_STRING;
            entry->name = "gateway";
            entry->value.s = buf;
            break;
        case 3:
            entry->type = MFS_INFO_SIZE;
            entry->name = "tx";
            entry->value.z = netif->tx;
            break;
        case 4:
            entry->type = MFS_INFO_SIZE;
            entry->name = "tx_drop";
            entry->value.z = netif->tx_drop;
            break;
        case 5:
            entry->type = MFS_INFO_SIZE;
            entry->name = "rx";
            entry->value.z = netif->rx;
            break;
        case 6:
            entry->type = MFS_INFO_SIZE;
            entry->name = "rx_drop";
            entry->value.z = netif->rx_drop;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op netif_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
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
    MFS_descriptor_t *dir = MFS_resolve("/sys/net/netif");
    MFS_descriptor_create (dir, name, MFS_SYS, &netif_descriptor_op, (MFS_represent_t *) netif);
    MFS_close_desc(dir);
}
