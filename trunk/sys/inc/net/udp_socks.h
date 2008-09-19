/*
 *  Copyright (C) 2007 Michael Feiner
 *
 */

#ifndef UDP_SOCKS_H
#define UDP_SOCKS_H

#include <ace/stddef.h>

#include "net/inet.h"
#include "net/ip.h"
#include "net/err.h"
#include "net/udp_sock.h"

/**
 * ?
 */
extern void NET_udp_socks_init (void);

/**
 * ?
 */
extern void
NET_udp_socks_lock (void);

/**
 * ?
 */
extern void
NET_udp_socks_unlock (void);

/**
 * ?
 */
extern NET_udp_socket_t *
NET_udp_find_sock (NET_netif_t * inp,
					 struct NET_ip_hdr *iphdr,
		             u16_t src,
				     u16_t dest);

/**
 * ?
 */
extern void
NET_udp_socks_add (NET_udp_socket_t * sock);

/**
 * ?
 */
extern void
NET_udp_socks_remove (NET_udp_socket_t * sock);

#endif /*UDP_SOCKS_H*/
