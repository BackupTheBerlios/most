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
#include "net/udp_socks.h"

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


extern void
NET_udp_socket_open (NET_udp_socket_t * sock)
{
	if (sock != NULL)
	{
		NET_udp_socks_add(sock);
	}
}

static NET_netbuf_t *
udp_recv_netbuf (NET_udp_socket_t * sock)
{
    NET_netbuf_t *p;
    if (sock->rx_timeout == TRUE)
        DEV_timer_start (&sock->rx_timer);
    p = (NET_netbuf_t *) USO_fetch (&sock->rx_que);
    if (p != NULL)
    {
        if (sock->rx_timeout == TRUE)
            DEV_timer_stop (&sock->rx_timer);
    }
	return p;
}

extern void
NET_udp_socket_close (NET_udp_socket_t * sock)
{
    NET_netbuf_t *p;
    if (sock != NULL)
    {
        NET_udp_socks_remove (sock);
        NET_udp_recv_timeout (sock, 1);
       	while ((p = udp_recv_netbuf (sock)) != NULL)
       	    NET_netbuf_free (p);
        NET_udp_recv_timeout (sock, 0);
    }
}

extern void
NET_udp_bind (NET_udp_socket_t * sock, NET_ip_addr_t * ipaddr, u16_t port)
{
    NET_ip_addr_set (&sock->local_ip, ipaddr);
    sock->local_port = port;
    DEBUGF (NET_UDP_DEBUG, ("Udp: bind port %d.\n", port));
}

extern void
NET_udp_connect (NET_udp_socket_t * sock, NET_ip_addr_t * ipaddr, u16_t port)
{
    NET_ip_addr_set (&sock->remote_ip, ipaddr);
    sock->remote_port = port;
    DEBUGF (NET_UDP_DEBUG, ("Udp: connect port %d.\n", port));
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

extern NET_netbuf_t *
NET_udp_recv_netbuf (NET_udp_socket_t * sock,
              		 NET_ip_addr_t * addr, u16_t * port)
{
	NET_netbuf_t *p;
    struct NET_udp_hdr *udphdr;
    struct NET_ip_hdr *iphdr;

	p = udp_recv_netbuf (sock);
	if (p != NULL){
    	udphdr = (struct NET_udp_hdr *)NET_netbuf_index(p);
		iphdr = (struct NET_ip_hdr *)((char*)udphdr - sizeof (struct NET_ip_hdr));
    	NET_netbuf_index_inc (p, sizeof (struct NET_udp_hdr));
    	if (addr != NULL)
    		*addr = iphdr->src;
    	if (port != NULL)
       		*port = ntohs (udphdr->src);
	}
    return p; 
}

extern long
NET_udp_recv (NET_udp_socket_t * sock,
              NET_ip_addr_t * addr, u16_t * port,
              char * buf, unsigned int len)
{
	NET_netbuf_t *p;
	int recv_len = 0;

	p = NET_udp_recv_netbuf (sock, addr, port);
	if (p != NULL){
    	recv_len = MIN(NET_netbuf_len(p), len);
    	memcpy (buf, NET_netbuf_index(p), recv_len);
   	 	NET_netbuf_free (p);
	}
    return recv_len; 
}

NET_err_t
NET_udp_send_netbuf (NET_udp_socket_t * sock, NET_netbuf_t *p)
{
    NET_netbuf_t *q;

    q = NET_netbuf_alloc_trans ();
    p = NET_netbuf_chain (q, p);

	return NET_udp_output(sock, p);
}

NET_err_t
NET_udp_send (NET_udp_socket_t * sock, char * data, unsigned int len)
{
    NET_netbuf_t *p;

	p = NET_netbuf_alloc_ram(len);
	memcpy (NET_netbuf_index(p), data, len);
	
    return NET_udp_send_netbuf (sock, p);
}
