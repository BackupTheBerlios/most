/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_UDP_H
#define NET_UDP_H

#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <uso/mailbox.h>
#include <dev/timer.h>

// #include "net/pbuf.h"
#include "net/inet.h"
#include "net/ip.h"
#include "net/err.h"

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

#define NET_UDP_FLAGS_NOCHKSUM 0x01 /**< No Checksum */
#define NET_UDP_FLAGS_UDPLITE  0x02 /**< UDP lite */

/** UDP Socket struct */
struct NET_udp_socket
{
    USO_node_t node;
    NET_ip_addr_t local_ip;
    NET_ip_addr_t remote_ip;
    u16_t local_port;
    u16_t remote_port;
    u8_t flags;
    u16_t chksum_len;
    USO_mailbox_t rx_que;
    DEV_timer_t rx_timer;
    bool_t rx_timeout;
};

/** UDP Socket */
typedef struct NET_udp_socket NET_udp_socket_t;

#define NET_udp_flags(sock)  ((sock)->flags) /**< ? */
#define NET_udp_setflags(sock, f)  ((sock)->flags = (f)) /**< ? */

/**
 * ?
 */
extern void NET_udp_init (void);

/**
 * ?
 */
extern NET_err_t NET_udp_input (NET_netif_t *, NET_netbuf_t *);

/**
 * ?
 */
extern void NET_udp_socket_init (NET_udp_socket_t *);

/**
 * ?
 */
extern NET_udp_socket_t *NET_udp_socket_new (void);

/**
 * ?
 */
extern void NET_udp_socket_open (NET_udp_socket_t *);

/**
 * ?
 */
extern void NET_udp_socket_close (NET_udp_socket_t *);

/**
 * ?
 */
extern void NET_udp_bind (NET_udp_socket_t *, NET_ip_addr_t *, u16_t);

/**
 * ?
 */
extern void NET_udp_connect (NET_udp_socket_t *, NET_ip_addr_t *, u16_t);

/**
 * ?
 */
extern void NET_udp_recv_timeout (NET_udp_socket_t *, unsigned long);

/**
 * ?
 */
extern NET_netbuf_t *NET_udp_recv (NET_udp_socket_t *,
                                   NET_ip_addr_t *, u16_t *);

/**
 * ?
 */
extern NET_err_t NET_udp_send (NET_udp_socket_t *, NET_netbuf_t *);

/** @}
 */

#endif
