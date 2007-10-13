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
print_ipaddr(char* name, NET_ip_addr_t ipaddr)
{
 	printf("\n\t%s: %3ld.%3ld.%3ld.%3ld.", 
			 name,
             ipaddr.addr >> 24 & 0xff,
             ipaddr.addr >> 16 & 0xff,
             ipaddr.addr >> 8 & 0xff,
             ipaddr.addr & 0xff);
 }

static void
info (MFS_entry_t *entry)
{
	NET_netif_t* netif = (NET_netif_t*) entry;

	print_ipaddr("ip_addr", netif->ip_addr);	
	print_ipaddr("netmask", netif->netmask);	
	print_ipaddr("gateway", netif->gateway);	
}

static struct MFS_descriptor_op netif_descriptor_op = {.open = NULL,
								        		      .close = NULL,
										              .info = info};

extern void
NET_netif_init (NET_netif_t * netif,
                char *name,
            	NET_err_t (*input) (NET_netif_t * netif, NET_netbuf_t * p) )
{
    netif->state = NULL;
    netif->input = input;
    NET_ip_addr_set (&(netif->ip_addr), &NET_ip_addr_any);
    NET_ip_addr_set (&(netif->netmask), &NET_ip_addr_any);
    NET_ip_addr_set (&(netif->gateway), &NET_ip_addr_any);
	MFS_create_desc(MFS_sysfs_netif(), name, (MFS_entry_t*) netif, MFS_DESC, &netif_descriptor_op);
}
