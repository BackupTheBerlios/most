/*
 *  Copyright (C) 2007 Michael Feiner
 *
 */

#ifndef UDP_SOCK_H
#define UDP_SOCK_H

#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <uso/mailbox.h>
#include <dev/timer.h>

#include "net/inet.h"
#include "net/ip.h"
#include "net/err.h"

/** UDP Socket */
typedef struct NET_udp_socket NET_udp_socket_t;

/** @defgroup udp_sock udp_sock.h
 *
 * User Datagramm Socket.
 *
 * @{
 */

#define NET_UDP_FLAGS_NOCHKSUM 0x01 /**< No Checksum */
#define NET_UDP_FLAGS_UDPLITE  0x02 /**< UDP lite */

/** UDP Socket struct */
struct NET_udp_socket
{
    USO_node_t node;
    NET_ip_addr_t local_ip;
    NET_ip_addr_t remote_ip;
    ACE_u16_t local_port;
    ACE_u16_t remote_port;
    ACE_u8_t flags;
    ACE_u16_t chksum_len;
    USO_mailbox_t rx_que;
    DEV_timer_t rx_timer;
    ACE_bool_t rx_timeout;
};

#define NET_udp_flags(sock)  ((sock)->flags) /**< ? */
#define NET_udp_setflags(sock, f)  ((sock)->flags = (f)) /**< ? */

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
extern void NET_udp_bind (NET_udp_socket_t *, NET_ip_addr_t *, ACE_u16_t);

/**
 * ?
 */
extern void NET_udp_connect (NET_udp_socket_t *, NET_ip_addr_t *, ACE_u16_t);

/**
 * ?
 */
extern void NET_udp_recv_timeout (NET_udp_socket_t *, unsigned long);

/**
 * ?
 */
extern NET_netbuf_t *
NET_udp_recv_netbuf (NET_udp_socket_t * sock,
              		 NET_ip_addr_t * addr, ACE_u16_t * port);

/**
 * ?
 */
extern long NET_udp_recv (NET_udp_socket_t *,
                          NET_ip_addr_t *, ACE_u16_t *,
                          char * buf, unsigned int len);

/**
 * ?
 */
NET_err_t
NET_udp_send_netbuf (NET_udp_socket_t * sock, NET_netbuf_t *p);

/**
 * ?
 */
extern NET_err_t NET_udp_send (NET_udp_socket_t * sock,
							   char * data,
							   unsigned int len);

/** @}
 */


#endif /*UDP_SOCK_H*/
