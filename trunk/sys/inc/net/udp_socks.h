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

/** @defgroup udp_socks udp_socks.h
 *
 * User datagramm socket list.
 *
 * @{
 */

/**
 * Initialize UDP socket list.
 */
extern void NET_udp_socks_init (void);

/**
 * Lock UDP mutex, used for synchronisation.
 */
extern void
NET_udp_socks_lock (void);

/**
 * Unlock UDP mutex, used for synchronisation.
 */
extern void
NET_udp_socks_unlock (void);

/**
 * Find appropriate socket.
 *
 * @param inp : Network interface for incoming packet.
 * @param iphdr : Pointer to IP header of incoming packet.
 * @param src : Source UDP port.
 * @param dest : Destination UDP port.
 * @return Found UDP socker or NULL.
 *
 */
extern NET_udp_socket_t *
NET_udp_find_sock (NET_netif_t * inp,
					 struct NET_ip_hdr *iphdr,
		             ACE_u16_t src,
				     ACE_u16_t dest);

/**
 * Add UDP socket to UDP socket list.
 */
extern void
NET_udp_socks_add (NET_udp_socket_t * sock);

/**
 * Remove UDP socket from UDP socket list.
 */
extern void
NET_udp_socks_remove (NET_udp_socket_t * sock);

/** @}
 */

#endif /*UDP_SOCKS_H*/
