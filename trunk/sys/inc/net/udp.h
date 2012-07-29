/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_UDP_H
#define NET_UDP_H

#include <ace/stddef.h>
#include <ace/stdlib.h>

#include "net/inet.h"
#include "net/ip.h"
#include "net/err.h"
#include "net/udp_sock.h"


/** @addtogroup net
 * @{
 */

/** @defgroup udp udp.h
 *
 * User Datagramm Protocol.
 *
 * @{
 */

#define NET_UDP_HLEN 8 /**< UDP Header Length. */

/** UDP Header structure. */
struct NET_udp_hdr
{
    ACE_u16_t src; /**< Source UDP port. */
    ACE_u16_t dest; /**< Destination UDP port. */
    ACE_u16_t len; /**< Length. */
    ACE_u16_t chksum; /**< Checksum. */
} ACE_PACKED_;

/**
 * Initialize UDP protocol.
 */
extern void NET_udp_init (void);

/**
 * UDP input function for incoming packets.
 * The index of netbuf p must point to the IP header.
 * @param inp : Network interface of incoming packet.
 * @param p : Incoming UDP packet.
 * @return NET_ERR.
 */
extern ACE_err_t NET_udp_input (NET_netif_t * inp, NET_netbuf_t * p);

/**
 * UDP output function for outgoing packets.
 * The index of netbuf p is decreased by the size of the UDP header and
 * the UDP header is added to p.
 * @param sock : UDP socket.
 * @param p : UDP packet which has to be sent.
 * @return NET_ERR.
 */
extern ACE_err_t NET_udp_output (NET_udp_socket_t * sock, NET_netbuf_t * p);

/** @}
 */

/** @}
 */


#endif
