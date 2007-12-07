/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>

#include "net/debug.h"
#include "net/opt.h"
#include "net/udp.h"
#include "net/udp_sock.h"

static USO_list_t udp_socks;

extern void
NET_udp_sock_init (void)
{
    USO_list_init (&udp_socks);
}

#define NET_UDP_RX_QUE_SIZE 10

extern void
NET_udp_socket_init (NET_udp_socket_t * sock)
{
    bzero (sock, sizeof (NET_udp_socket_t));
    USO_mailbox_init (&sock->rx_que, NET_UDP_RX_QUE_SIZE);
    sock->rx_timeout = FALSE;
}

extern NET_udp_socket_t *
NET_udp_socket_new (void)
{
    NET_udp_socket_t *sock;
    sock = malloc (sizeof (NET_udp_socket_t));
    if (sock != NULL)
    {
        NET_udp_socket_init (sock);
    }
    return sock;
}

extern NET_udp_socket_t *
NET_find_exact_sock (NET_netif_t * inp,
					 struct NET_ip_hdr *iphdr,
		             u16_t src,
			         u16_t dest)
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
            DEBUGF (NET_UDP_DEBUG, ("\nUdp: found exact sock."));
            break;
        }
    }
    return sock;
}

extern NET_udp_socket_t *
NET_find_sock (struct NET_ip_hdr *iphdr,
			   u16_t src,
			   u16_t dest)
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
        	DEBUGF (NET_UDP_DEBUG, ("\nUdp: found sock."));
            break;
        }
    }
	return sock;
}

static NET_udp_socket_t *
find_sock (NET_udp_socket_t * sock)
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
NET_udp_socket_open (NET_udp_socket_t * sock)
{
    NET_udp_socket_t *isock;
    isock = find_sock(sock);
    if (isock == NULL)
    {
        USO_enqueue (&udp_socks, (USO_node_t *) sock);
    }
}

extern void
NET_udp_socket_close (NET_udp_socket_t * sock)
{
    NET_udp_socket_t *isock;
    //NET_netbuf_t *packet;
    //NET_ip_addr_t addr;
    //u16_t port;
    isock = find_sock(sock);
    if (isock != NULL)
    {
        USO_remove (&udp_socks, (USO_node_t *) sock);
        NET_udp_recv_timeout (sock, 1);
       // while ((packet = NET_udp_recv (sock, &addr, &port, NULL, 0)) != NULL)
       //     NET_netbuf_free (packet);
        NET_udp_recv_timeout (sock, 0);
    }
}

extern void
NET_udp_bind (NET_udp_socket_t * sock, NET_ip_addr_t * ipaddr, u16_t port)
{
    NET_ip_addr_set (&sock->local_ip, ipaddr);
    sock->local_port = port;
    DEBUGF (NET_UDP_DEBUG, ("\nUdp: bind port %d.", port));
}

extern void
NET_udp_connect (NET_udp_socket_t * sock, NET_ip_addr_t * ipaddr, u16_t port)
{
    NET_ip_addr_set (&sock->remote_ip, ipaddr);
    sock->remote_port = port;
    DEBUGF (NET_UDP_DEBUG, ("\nUdp: connect port %d.", port));
}

static void
rx_timeout (void *rx_que)
{
    USO_fetch_abort ((USO_mailbox_t *) rx_que);
}

extern void
NET_udp_recv_timeout (NET_udp_socket_t * sock, unsigned long timeout)
{
    if (timeout > 0)
    {
        DEV_timer_init (&sock->rx_timer, rx_timeout, &sock->rx_que, timeout);
        sock->rx_timeout = TRUE;
    }
    else
    {
        sock->rx_timeout = FALSE;
    }
}

/*
 * timeout has to be implemented 
 */
extern long
NET_udp_recv (NET_udp_socket_t * sock,
              NET_ip_addr_t * addr, u16_t * port,
              char * buf, unsigned int len)
{
    NET_netbuf_t *p;
    struct NET_udp_hdr *udphdr;
    struct NET_ip_hdr *iphdr;
    if (sock->rx_timeout == TRUE)
        DEV_timer_start (&sock->rx_timer);
    p = (NET_netbuf_t *) USO_fetch (&sock->rx_que);
    if (p != NULL)
    {
        if (sock->rx_timeout == TRUE)
            DEV_timer_stop (&sock->rx_timer);
        iphdr = (struct NET_ip_hdr *)(p->index - sizeof (struct NET_ip_hdr));
        udphdr = (struct NET_udp_hdr *)p->index;
        NET_netbuf_index_inc (p, sizeof (struct NET_udp_hdr));
        if (addr != NULL)
            *addr = iphdr->src;
        if (port != NULL)
            *port = ntohs (udphdr->src);
    }
    int recv_len = MIN(p->len, len);
    memcpy (buf, p->index, recv_len);
    NET_netbuf_free (p); //????
    return recv_len; 
}

NET_err_t
NET_udp_send (NET_udp_socket_t * sock, char * data, unsigned int len)
{
    NET_err_t err;
    NET_netbuf_t *p, *q;

	p = NET_netbuf_alloc_rom(data, len);
    q = NET_netbuf_alloc_trans ();
    p = NET_netbuf_chain (q, p);

	err = NET_udp_output(sock, p);
	
	NET_netbuf_free (p);
    return err;
}
