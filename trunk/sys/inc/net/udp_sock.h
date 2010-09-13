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

/** @defgroup udp_sock udp_sock.h
 *
 * User Datagramm Socket.
 *
 * @{
 */

/** UDP socket type. */
typedef struct NET_udp_socket NET_udp_socket_t;


#define NET_UDP_FLAGS_NOCHKSUM 0x01 /**< No checksum. */
#define NET_UDP_FLAGS_UDPLITE  0x02 /**< UDP lite. */

/** UDP Socket structure. */
struct NET_udp_socket
{
    USO_node_t node;               /**< List node. */
    NET_ip_addr_t local_ip;        /**< Local IP address. */
    NET_ip_addr_t remote_ip;       /**< Remote IP address. */
    ACE_u16_t local_port;          /**< Local port. */
    ACE_u16_t remote_port;         /**< Remote port. */
    ACE_u8_t flags;                /**< Flags. */
    ACE_u16_t chksum_len;          /**< Checksum and length. */
    USO_mailbox_t rx_que;          /**< Receive mailbox. */
    DEV_timer_t rx_timer;          /**< Receive timer. */
    ACE_bool_t rx_timeout;         /**< Receive timeout. */
};

#define NET_udp_flags(sock)  ((sock)->flags) /**< Get UDP socket flags. */
#define NET_udp_setflags(sock, f)  ((sock)->flags = (f)) /**< Set UDP socket flags. */

/**
 * Initialize UDP socket.
 * @param sock : UDP socket.
 */
extern void NET_udp_socket_init (NET_udp_socket_t *sock);

/**
 * Create and initialize a UDP socket.
 * @return Created UDP socket.
 */
extern NET_udp_socket_t *NET_udp_socket_new (void);

/**
 * Add UDP socket to list of UDP sockets.
 * @param sock : UDP socket.
 */
extern void NET_udp_socket_open (NET_udp_socket_t *sock);

/**
 * Remove UDP socket from list of UDP sockets and release resources.
 * @param sock : UDP socket.
 */
extern void NET_udp_socket_close (NET_udp_socket_t *sock);

/**
 * Bind the UDP socket to a local IP address and port.
 * @param sock : UDP socket.
 * @param ipaddr : Local IP address.
 * @param port : Local port.
 */
extern void NET_udp_bind (NET_udp_socket_t *sock, NET_ip_addr_t *ipaddr, ACE_u16_t port);

/**
 * Bind(connect) the UDP socket to a remote IP address and port.
 * @param sock : UDP socket.
 * @param ipaddr : Remote IP address.
 * @param port : Remote port.
 */
extern void NET_udp_connect (NET_udp_socket_t *sock, NET_ip_addr_t *ipaddr, ACE_u16_t port);

/**
 * Set UDP receive timeout.
 * @param sock : UDP socket.
 * @param timeout : Receive timeout.
 */
extern void NET_udp_recv_timeout (NET_udp_socket_t *sock, unsigned long timeout);

/**
 * Receive a UDP packet as network buffer.
 * The netbuf index points to the payload data.
 * The user must take care about releasing the network buffer.
 * @param socket : UDP socket from which the buffer should be received.
 * @param ipaddr : Get back the senders (source) IP address.
 * @param port : Get back the senders (source) port.
 * @return Received UDP packet.
 */
extern NET_netbuf_t *
NET_udp_recv_netbuf (NET_udp_socket_t * sock,
              		 NET_ip_addr_t * ipaddr, ACE_u16_t * port);

/**
 * Receive a UDP packet.
 * @param socket : UDP socket from which the buffer should be received.
 * @param ipaddr : Get back the senders (source) IP address.
 * @param port : Get back the senders (source) port.
 * @param buf : Pointer to buffer where the received data is copied.
 * @param len : Maximal received data length.
 * @return Real received data length.
 */
extern long NET_udp_recv (NET_udp_socket_t *sock,
                          NET_ip_addr_t *ipaddr, ACE_u16_t * port,
                          char * buf, unsigned int len);

/**
 * Send a network buffer as UDP packet.
 * The buffer will be chained to a transfer buffer.
 * !Currently the data length is not checked!
 * @param sock : UDP socket for sending the packet.
 * @param p : UDP packet as netbuf.
 * @return NET_ERR.
 */
NET_err_t
NET_udp_send_netbuf (NET_udp_socket_t *sock, NET_netbuf_t *p);

/**
 * Send a network buffer as UDP packet.
 * The data is copied to a network buffer.
 * !Currently the data length is not checked!
 * @param sock : UDP socket for sending the packet.
 * @param data : Pointer to data for sending.
 * @param len : Data length.
 * @return NET_ERR.
 */
extern NET_err_t NET_udp_send (NET_udp_socket_t * sock,
							   char * data,
							   unsigned int len);

/** @}
 */


#endif /*UDP_SOCK_H*/
