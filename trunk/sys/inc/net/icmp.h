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

#define NET_ICMP_ER 0           /**< echo reply */
#define NET_ICMP_DUR 3          /**< destination unreachable */
#define NET_ICMP_SQ 4           /**< source quench */
#define NET_ICMP_RD 5           /**< redirect */
#define NET_ICMP_ECHO 8         /**< echo */
#define NET_ICMP_TE 11          /**< time exceeded */
#define NET_ICMP_PP 12          /**< parameter problem */
#define NET_ICMP_TS 13          /**< timestamp */
#define NET_ICMP_TSR 14         /**< timestamp reply */
#define NET_ICMP_IRQ 15         /**< information request */
#define NET_ICMP_IR 16          /**< information reply */

/**
 * ?.
 */
enum NET_icmp_dur_type
{
    NET_ICMP_DUR_NET = 0,       /**< net unreachable */
    NET_ICMP_DUR_HOST = 1,      /**< host unreachable */
    NET_ICMP_DUR_PROTO = 2,     /**< protocol unreachable */
    NET_ICMP_DUR_PORT = 3,      /**< port unreachable */
    NET_ICMP_DUR_FRAG = 4,      /**< fragmentation needed and DF set */
    NET_ICMP_DUR_SR = 5         /**< source route failed */
};

/** ?. */
enum NET_icmp_te_type
{
    NET_ICMP_TE_TTL = 0,        /**< time to live exceeded in transit */
    NET_ICMP_TE_FRAG = 1        /**< fragment reassembly time exceeded */
};

/**
 * 
 */
extern NET_err_t NET_icmp_input (NET_netbuf_t *, NET_netif_t *);

/**
 * 
 */
extern void NET_icmp_dest_unreach (NET_netbuf_t *, enum NET_icmp_dur_type);

//extern void NET_icmp_time_exceeded (NET_netbuf_t *, enum NET_icmp_te_type);

/** ? */
struct NET_icmp_echo_hdr
{
    ACE_u16_t type_code;  /**<  */
    ACE_u16_t chksum;   /**<  */
    ACE_u16_t id;  /**<  */
    ACE_u16_t seqno; /**<  */
} ACE_PACKED_;

/** ? */
struct NET_icmp_dur_hdr
{
    ACE_u16_t type_code; /**< ?. */
    ACE_u16_t chksum;  /**< ?. */
    ACE_u32_t unused; /**< ?. */
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

#define NET_ICMPH_TYPE(hdr) (ACE_ntohs((hdr)->type_code) >> 8)   /**< ?. */
#define NET_ICMPH_CODE(hdr) (ACE_ntohs((hdr)->type_code) & 0xff)  /**< ?. */

/** ?. */
#define NET_ICMPH_TYPE_SET(hdr, type) ((hdr)->type_code = ACE_htons(NET_ICMPH_CODE(hdr) | ((type) << 8)))

/** ?. */
#define NET_ICMPH_CODE_SET(hdr, code) ((hdr)->type_code = ACE_htons((code) | (NET_ICMPH_TYPE(hdr) << 8)))

/** @}
 */

#endif
