/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_INET_H
#define NET_INET_H

#include <ace/stddef.h>

#include "net/netbuf.h"
#include "net/ip_addr.h"

/** @addtogroup net
 * @{
 */

/** @defgroup inet inet.h
 *
 * Calculate the internet checksum.
 *
 * @{
 */

/**
 * Calculates the Internet checksum over a portion of
 * memory. Used primarely for IP and ICMP.
 * @param dataptr : Pointer to data for checksum calculation.
 * @param len : Length of data for.
 * @return Checksum (as u16_t).
 */
extern ACE_u16_t NET_inet_chksum (void *dataptr, ACE_u16_t);

/**
 * Calculate a checksum over a chain of netbufs (without pseudo-header, much like
 * inet_chksum only netbufs are used).
 *
 * @param p : netbuf (packet) chain over that the checksum should be calculated
 * @return Checksum (as u16_t) to be saved directly in the protocol header.
 */
extern ACE_u16_t NET_inet_chksum_buf (NET_netbuf_t * p);

/** Inet_chksum_pseudo:
 *
 * Calculates the pseudo Internet checksum used by TCP and UDP for a netbuf chain.
 * IP addresses are expected to be in network byte order.
 *
 * @param p : Chain of netbufs over that a checksum should be calculated (ip data part).
 * @param src : Source IP address (used for checksum of pseudo header).
 * @param dst : Destination IP address (used for checksum of pseudo header).
 * @param proto : IP protocol (used for checksum of pseudo header).
 * @param proto_len : Length of the IP data part (used for checksum of pseudo header):
 * @return Checksum (as u16_t) to be saved directly in the protocol header.
 */
extern ACE_u16_t NET_inet_chksum_pseudo (NET_netbuf_t * p,
                                         NET_ip_addr_t * src,
                                         NET_ip_addr_t * dst, ACE_u8_t proto, ACE_u16_t proto_len);

/** @}
 */

/** @}
 */


#endif
