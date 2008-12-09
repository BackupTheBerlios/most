/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/mutex.h>

#include "net/debug.h"
#include "net/udp_socks.h"

static USO_list_t udp_socks;
static USO_mutex_t udp_socks_mutex;

extern void
NET_udp_socks_init (void)
{
    USO_list_init (&udp_socks);
    USO_mutex_init (&udp_socks_mutex);
}

extern void
NET_udp_socks_lock (void)
{
	USO_lock(&udp_socks_mutex);
}

extern void
NET_udp_socks_unlock (void)
{
	USO_unlock(&udp_socks_mutex);
}

static NET_udp_socket_t *
udp_find_exact_sock (NET_netif_t * inp,
					 struct NET_ip_hdr *iphdr,
		             ACE_u16_t src,
			         ACE_u16_t dest)
{
	NET_udp_socket_t *sock = NULL;
	
	while ((sock = (NET_udp_socket_t *) USO_next_element (&udp_socks, (USO_node_t*)sock)) != NULL)
	{
        if (sock->remote_port == src &&
            sock->local_port == dest &&
            (NET_ip_addr_isbroadcast ((&sock->remote_ip), &(inp->netmask)) ||
             NET_ip_addr_isany (&sock->remote_ip) ||
             NET_ip_addr_cmp (&(sock->remote_ip), &(iphdr->src))) &&
            (NET_ip_addr_isany (&sock->local_ip) ||
             NET_ip_addr_cmp (&(sock->local_ip), &(iphdr->dest))))
        {
            DEBUGF (NET_UDP_DEBUG, ("Udp: found exact sock.\n"));
            break;
        }
    }
    return sock;
}

static NET_udp_socket_t *
udp_find_sock (struct NET_ip_hdr *iphdr,
			   ACE_u16_t src,
			   ACE_u16_t dest)
{
	NET_udp_socket_t *sock = NULL;
	
    while ((sock = (NET_udp_socket_t *) USO_next_element (&udp_socks, (USO_node_t*)sock)) != NULL)
    {
    	if (sock->local_port == dest &&
    	    (NET_ip_addr_isany (&sock->remote_ip) ||
    	     NET_ip_addr_cmp (&(sock->remote_ip), &(iphdr->src))) &&
    	    (NET_ip_addr_isany (&sock->local_ip) ||
    	     NET_ip_addr_cmp (&(sock->local_ip), &(iphdr->dest))))
        {
        	DEBUGF (NET_UDP_DEBUG, ("Udp: found sock.\n"));
            break;
        }
    }
	return sock;
}

extern NET_udp_socket_t *
NET_udp_find_sock (NET_netif_t * inp,
					 struct NET_ip_hdr *iphdr,
		             ACE_u16_t src,
			         ACE_u16_t dest)
{
	NET_udp_socket_t *sock = NULL;

	sock = udp_find_exact_sock(inp, iphdr, src, dest);
 	if (sock == NULL)
    {
    	sock = udp_find_sock(iphdr, src, dest);
    }
	return sock;
}


static NET_udp_socket_t *
udp_has_sock (NET_udp_socket_t * sock)
{
	NET_udp_socket_t *isock = NULL;
	while ( (isock = (NET_udp_socket_t*)USO_next_element(&udp_socks, (USO_node_t*)isock))
			!= NULL){
	    if (sock == isock){
   			break;	   	
    	}		   		
	}    
	return isock;
}

extern void
NET_udp_socks_add (NET_udp_socket_t * sock)
{
    NET_udp_socket_t *isock;
	USO_lock(&udp_socks_mutex);
    isock = udp_has_sock(sock);
    if (isock == NULL)
    {
        USO_enqueue (&udp_socks, (USO_node_t *) sock);
    }
	USO_unlock(&udp_socks_mutex);
}

extern void
NET_udp_socks_remove (NET_udp_socket_t * sock)
{
    NET_udp_socket_t *isock;
	USO_lock(&udp_socks_mutex);
    isock = udp_has_sock(sock);
    if (isock != NULL)
    {
    	USO_remove (&udp_socks, (USO_node_t *) sock);
    }
	USO_unlock(&udp_socks_mutex);
}
