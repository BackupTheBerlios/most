/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ICMP_H
#define NET_ICMP_H

#include <ace/stddef.h>
#include <ace/stdlib.h>

#include "net/opt.h"
#include "net/netif.h"

/** @defgroup icmp icmp.h
 *
 * Internet control message protocol.
 *
 * @{
 */

#define NET_ICMP_ER 0           /**< Echo reply. */
#define NET_ICMP_DUR 3          /**< Destination unreachable. */
#define NET_ICMP_SQ 4           /**< Source quench. */
#define NET_ICMP_RD 5           /**< Redirect. */
#define NET_ICMP_ECHO 8         /**< Echo. */
#define NET_ICMP_TE 11          /**< Time exceeded. */
#define NET_ICMP_PP 12          /**< Parameter problem. */
#define NET_ICMP_TS 13          /**< Timestamp. */
#define NET_ICMP_TSR 14         /**< Timestamp reply. */
#define NET_ICMP_IRQ 15         /**< Information request. */
#define NET_ICMP_IR 16          /**< Information reply. */

/**
 * Duration type.
 */
enum NET_icmp_dur_type
{
    NET_ICMP_DUR_NET = 0,       /**< Net unreachable. */
    NET_ICMP_DUR_HOST = 1,      /**< Host unreachable. */
    NET_ICMP_DUR_PROTO = 2,     /**< Protocol unreachable. */
    NET_ICMP_DUR_PORT = 3,      /**< Port unreachable. */
    NET_ICMP_DUR_FRAG = 4,      /**< Fragmentation needed and DF set. */
    NET_ICMP_DUR_SR = 5         /**< Source route failed. */
};

/** Time exeeded type. */
enum NET_icmp_te_type
{
    NET_ICMP_TE_TTL = 0,        /**< Time to live exceeded in transit. */
    NET_ICMP_TE_FRAG = 1        /**< Fragment reassembly time exceeded. */
};

/**
 * An ICMP packet is received.
 * @param p : Packet.
 * @param inp : Network interface where p is received.
 * @return NET_ERR.
 */
extern NET_err_t NET_icmp_input (NET_netbuf_t *p, NET_netif_t *inp);

/**
 * Send ICMP destination unreachable.
 * @param p : Received packet.
 * @param t : ICMP duration type.
 */
extern void NET_icmp_dest_unreach (NET_netbuf_t *p, enum NET_icmp_dur_type t);

//extern void NET_icmp_time_exceeded (NET_netbuf_t *, enum NET_icmp_te_type);

/** Echo header. */
struct NET_icmp_echo_hdr
{
    ACE_u16_t type_code;  /**< ICMP packet echo type and code. */
    ACE_u16_t chksum;     /**< Checksum. */
    ACE_u16_t id;         /**<  ? */
    ACE_u16_t seqno;      /**< Sequence number. */
} ACE_PACKED_;

/** ? */
struct NET_icmp_dur_hdr
{
    ACE_u16_t type_code; /**< ICMP packet duration type and code. */
    ACE_u16_t chksum;  /**< Checksum. */
    ACE_u32_t unused; /**< Unused. */
} ACE_PACKED_;

/*
#if 0
struct NET_icmp_te_hdr
{
    ACE_u16_t type_code;
    ACE_u16_t chksum;
    ACE_u32_t unused;
} ACE_PACKED_;
#endif
*/

#define NET_ICMPH_TYPE(hdr) (ACE_ntohs((hdr)->type_code) >> 8)   /**< Get ICMP type from header. */
#define NET_ICMPH_CODE(hdr) (ACE_ntohs((hdr)->type_code) & 0xff)  /**< Get ICMP code from header. */

/** Set ICMP type to header. */
#define NET_ICMPH_TYPE_SET(hdr, type) ((hdr)->type_code = ACE_htons(NET_ICMPH_CODE(hdr) | ((type) << 8)))

/** Set ICMP code to header. */
#define NET_ICMPH_CODE_SET(hdr, code) ((hdr)->type_code = ACE_htons((code) | (NET_ICMPH_TYPE(hdr) << 8)))

/** @}
 */

#endif
