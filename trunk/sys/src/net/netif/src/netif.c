/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>

#include "net/debug.h"
#include "net/netif.h"

USO_list_t NET_netif_list;
NET_netif_t *NET_netif_default = NULL;


void
NET_netif_list_init (void)
{
    USO_list_init (&NET_netif_list);
}

extern void
NET_netif_init (NET_netif_t * netif,
                char *name,
                NET_ip_addr_t * ipaddr,
                NET_ip_addr_t * netmask,
                NET_ip_addr_t * gateway,
                NET_err_t (*input) (NET_netif_t * netif, NET_netbuf_t * p))
{
    netif->state = NULL;
    netif->name = name;
    netif->input = input;
    NET_ip_addr_set (&(netif->ip_addr), ipaddr);
    NET_ip_addr_set (&(netif->netmask), netmask);
    NET_ip_addr_set (&(netif->gateway), gateway);
    USO_enqueue (&NET_netif_list, (USO_node_t *) netif);
}

static NET_netif_t *
find(char* name)
{
	NET_netif_t *netif = NULL;
	while ( (netif = (NET_netif_t*)USO_next_element(&NET_netif_list, (USO_node_t*)netif))
			!= NULL){
	    if (strcmp ( netif->name, name) == 0 ){
   			break;	   	
    	}		   		
	}    
	return netif;
}

NET_netif_t *
NET_netif_find (char *name)
{
    NET_netif_t *netif;
    netif = find(name);
    if (netif != NULL)
    {
        DEBUGF (NET_NETIF_DEBUG, ("\nNetif: found %s.", name));
    }
    else
    {
        DEBUGF (NET_NETIF_DEBUG, ("\nNetif: not found %s.", name));
    }
    return netif;
}

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
