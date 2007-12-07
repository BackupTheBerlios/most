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


/** @defgroup udp udp.h
 *
 * User Datagramm Protocol.
 *
 * @{
 */

#define NET_UDP_HLEN 8 /**< UDP Header Length */

/** UDP Header */
struct NET_udp_hdr
{
    u16_t src; /**< src UDP port */
    u16_t dest; /**< dest UDP port */
    u16_t len; /**< length */
    u16_t chksum; /**< checksum */
} _PACKED_;

/**
 * ?
 */
extern void NET_udp_init (void);

/**
 * ?
 */
extern NET_err_t NET_udp_input (NET_netif_t *,
								NET_netbuf_t *);

/**
 * ?
 */
extern NET_err_t NET_udp_output (NET_udp_socket_t *,
								 NET_netbuf_t *);

/** @}
 */

#endif
