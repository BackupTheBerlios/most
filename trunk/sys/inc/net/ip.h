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

#define NET_IP_HLEN 20 /**< IP Header length. */

#define NET_IP_PROTO_ICMP 1  /**< ICMP Protocol. */
#define NET_IP_PROTO_UDP 17  /**< UDP Protocol. */
#define NET_IP_PROTO_UDPLITE 170 /** UDPLITE Protocol. */
#define NET_IP_PROTO_TCP 6 /* TCP Protocol. */

/*
 * This is passed as the destination address to ip_output_if (not to
 * ip_output), meaning that an IP header already is constructed in the
 * netbuf. This is used when TCP retransmits. 
 */
#ifdef NET_IP_HDRINCL
#undef NET_IP_HDRINCL
#endif /* IP_HDRINCL */
#define NET_IP_HDRINCL  NULL

#define NET_IP_RF 0x8000        /**< Reserved fragment flag. */
#define NET_IP_DF 0x4000        /**< Ddont fragment flag. */
#define NET_IP_MF 0x2000        /**< More fragments flag. */
#define NET_IP_OFFMASK 0x1fff   /**< Mask for fragmenting bits. */

/** IP Header. */
struct NET_ip_hdr
{
    ACE_u16_t v_hl_tos;    /**< Version / header length / type of service. */
    ACE_u16_t len;         /**< Total length. */
    ACE_u16_t id;          /**< Identification. */
    ACE_u16_t offset;      /**< Fragment offset field. */
    ACE_u16_t ttl_proto;   /**< Time to live / protocol. */
    ACE_u16_t chksum;      /**< Checksum. */
    NET_ip_addr_t src;     /**< Source IP addresses. */
    NET_ip_addr_t dest;    /**< Destination IP addresses. */
} ACE_PACKED_;


/** Get IP header: version. */
#define NET_IPH_V(hdr)  (ACE_ntohs((hdr)->v_hl_tos) >> 12)

/** Get IP header: header length. */
#define NET_IPH_HL(hdr) ((ACE_ntohs((hdr)->v_hl_tos) >> 8) & 0x0f)

/** Get IP header: type of service. */
#define NET_IPH_TOS(hdr) ACE_htons((ACE_ntohs((hdr)->v_hl_tos) & 0xff))

/** Get IP header: total length. */
#define NET_IPH_LEN(hdr) ((hdr)->len)

/** Get IP header: identification. */
#define NET_IPH_ID(hdr) ((hdr)->id)

/** Get IP header: fragment offset. */
#define NET_IPH_OFFSET(hdr) ((hdr)->offset)

/** Get IP header: time to live. */
#define NET_IPH_TTL(hdr) (ACE_ntohs((hdr)->ttl_proto) >> 8)

/** Get IP header: protocol. */
#define NET_IPH_PROTO(hdr) (ACE_ntohs((hdr)->ttl_proto) & 0xff)

/** Get IP header: checksum. */
#define NET_IPH_CHKSUM(hdr) ((hdr)->chksum)


/** Set IP header: version, header length, type of service. */
#define NET_IPH_VHLTOS_SET(hdr, v, hl, tos) (hdr)->v_hl_tos = ACE_htons(((v) << 12) | ((hl) << 8) | (tos))

/** Set IP header: total length. */
#define NET_IPH_LEN_SET(hdr, l) (hdr)->len = (l)

/** Set IP header: identification. */
#define NET_IPH_ID_SET(hdr, i) (hdr)->id = (i)

/** Set IP header: fragment offset. */
#define NET_IPH_OFFSET_SET(hdr, off) (hdr)->offset = (off)

/** Set IP header: time to live. */
#define NET_IPH_TTL_SET(hdr, ttl) (hdr)->ttl_proto = ACE_htons(NET_IPH_PROTO(hdr) | ((ttl) << 8))

/** Set IP header: protocol. */
#define NET_IPH_PROTO_SET(hdr, proto) (hdr)->ttl_proto = ACE_htons((proto) | (NET_IPH_TTL(hdr) << 8))

/** Set IP header: checksum. */
#define NET_IPH_CHKSUM_SET(hdr, chk) (hdr)->chksum = (chk)


/**
 * Initialize the IP layer.
 */
extern void NET_ip_init (void);

/**
 * Find an interface for a given IP address.
 * @param dest : Destination IP address.
 * @return Network interface for transmitting the packet.
 */
extern NET_netif_t *NET_ip_route (NET_ip_addr_t *dest);

/**
 * Ip_input: This function is called by the network interface device
 * driver when an IP packet is received.
 * @param inp : Network interface where the packet was received.
 * @param p : Received packet.
 * @return NET_ERR.
 */
extern NET_err_t NET_ip_input (NET_netif_t *inp, NET_netbuf_t *p);

/**
 * Ip_output: Simple interface to ip_output_if. It finds the outgoing
 * network interface and calls upon ip_output_if to do the actual work.
 * @param p : Packet to send.
 * @param src : Source IP address.
 * @param dest : Destination IP address.
 * @param ttl : Time to live.
 * @param proto : Protocol.
 * @return NET_ERR.
 */
NET_err_t
NET_ip_output (NET_netbuf_t * p,
               NET_ip_addr_t * src,
               NET_ip_addr_t * dest,
               ACE_u8_t ttl, ACE_u8_t proto);

/**
 * Ip_output_if: Sends an IP packet on a network interface. This function
 * constructs the IP header and calculates the IP header checksum. If the
 * source IP address is NULL, the IP address of the outgoing network
 * interface is filled in as source address.
 * @param p : Packet to send.
 * @param src : Source IP address.
 * @param dest : Destination IP address.
 * @param ttl : Time to live.
 * @param proto : Protocol.
 * @param netif : Network interface for transmitting the packet.
 * @return NET_ERR.
 */
extern NET_err_t
NET_ip_output_if (NET_netbuf_t * p,
                  NET_ip_addr_t * src,
                  NET_ip_addr_t * dest,
                  ACE_u8_t ttl, ACE_u8_t proto,
                  NET_netif_t * netif);

/** @}
 */

#endif
