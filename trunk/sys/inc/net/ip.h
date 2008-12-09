/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_IP_H
#define NET_IP_H

#include <ace/stddef.h>
#include <ace/stdlib.h>

#include "net/ip_addr.h"
#include "net/netif.h"
#include "net/err.h"

/** @defgroup ip ip.h
 *
 * Internet Protocol.
 *
 * @{
 */

#define NET_IP_HLEN 20 /**< IP Header length */

#define NET_IP_PROTO_ICMP 1  /**< ICMP Protocol */
#define NET_IP_PROTO_UDP 17  /**< UDP Protocol */
#define NET_IP_PROTO_UDPLITE 170 /** UDPLITE Protocol */
#define NET_IP_PROTO_TCP 6 /* TCP Protocol */ 

/*
 * This is passed as the destination address to ip_output_if (not to
 * ip_output), meaning that an IP header already is constructed in the
 * netbuf. This is used when TCP retransmits. 
 */
#ifdef NET_IP_HDRINCL
#undef NET_IP_HDRINCL
#endif /* IP_HDRINCL */
#define NET_IP_HDRINCL  NULL

#define NET_IP_RF 0x8000        /**< reserved fragment flag */
#define NET_IP_DF 0x4000        /**< dont fragment flag */
#define NET_IP_MF 0x2000        /**< more fragments flag */
#define NET_IP_OFFMASK 0x1fff   /**< mask for fragmenting bits */

/** IP Header */
struct NET_ip_hdr
{
    ACE_u16_t v_hl_tos;    /**< version / header length / type of service */
    ACE_u16_t len;     /**< total length */
    ACE_u16_t id;     /**< identification */ 
    ACE_u16_t offset;    /**< fragment offset field */
    ACE_u16_t ttl_proto;     /**< time to live / protocol */
    ACE_u16_t chksum;   /**< checksum */
    NET_ip_addr_t src;    /**< source IP addresses */
    NET_ip_addr_t dest;    /**< destination IP addresses */
} ACE_PACKED_;


/** ? */
#define NET_IPH_V(hdr)  (ACE_ntohs((hdr)->v_hl_tos) >> 12)

/** ? */
#define NET_IPH_HL(hdr) ((ACE_ntohs((hdr)->v_hl_tos) >> 8) & 0x0f)

/** ? */
#define NET_IPH_TOS(hdr) ACE_htons((ACE_ntohs((hdr)->v_hl_tos) & 0xff))

/** ? */
#define NET_IPH_LEN(hdr) ((hdr)->len)

/** ? */
#define NET_IPH_ID(hdr) ((hdr)->id)

/** ? */
#define NET_IPH_OFFSET(hdr) ((hdr)->offset)

/** ? */
#define NET_IPH_TTL(hdr) (ACE_ntohs((hdr)->ttl_proto) >> 8)

/** ? */
#define NET_IPH_PROTO(hdr) (ACE_ntohs((hdr)->ttl_proto) & 0xff)

/** ? */
#define NET_IPH_CHKSUM(hdr) ((hdr)->chksum)


/** ? */
#define NET_IPH_VHLTOS_SET(hdr, v, hl, tos) (hdr)->v_hl_tos = ACE_htons(((v) << 12) | ((hl) << 8) | (tos))

/** ? */
#define NET_IPH_LEN_SET(hdr, l) (hdr)->len = (l)

/** ? */
#define NET_IPH_ID_SET(hdr, i) (hdr)->id = (i)

/** ? */
#define NET_IPH_OFFSET_SET(hdr, off) (hdr)->offset = (off)

/** ? */
#define NET_IPH_TTL_SET(hdr, ttl) (hdr)->ttl_proto = ACE_htons(NET_IPH_PROTO(hdr) | ((ttl) << 8))

/** ? */
#define NET_IPH_PROTO_SET(hdr, proto) (hdr)->ttl_proto = ACE_htons((proto) | (NET_IPH_TTL(hdr) << 8))

/** ? */
#define NET_IPH_CHKSUM_SET(hdr, chk) (hdr)->chksum = (chk)


/**
 * Initialize.
 */
extern void NET_ip_init (void);

/**
 * Route.
 */
extern NET_netif_t *NET_ip_route (NET_ip_addr_t *);

/**
 * Input.
 */
extern NET_err_t NET_ip_input (NET_netif_t *, NET_netbuf_t *);

/**
 * Output.
 */
extern NET_err_t NET_ip_output (NET_netbuf_t *,
                         NET_ip_addr_t *, NET_ip_addr_t *, ACE_u8_t, ACE_u8_t);

/**
 * Output If.
 */
extern NET_err_t NET_ip_output_if (NET_netbuf_t *,
                                   NET_ip_addr_t *,
                                   NET_ip_addr_t *, ACE_u8_t, ACE_u8_t, NET_netif_t *);

/** @}
 */

#endif
