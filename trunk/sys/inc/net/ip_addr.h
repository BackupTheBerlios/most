/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_IP_ADDR_H
#define NET_IP_ADDR_H

#include <ace/stddef.h>
#include <ace/stdlib.h>

#include "net/debug.h"

/** @defgroup ip_addr ip_addr.h
 *
 * Internet Protocol Address.
 *
 * @{
 */

/** IP Address Struct. */
struct NET_ip_addr
{
    ACE_u32_t addr; /**< Address. */
} ACE_PACKED_;

/** IP Address Type. */
typedef struct NET_ip_addr NET_ip_addr_t;

#define NET_IP_ADDR_ANY 0 /**< Define for Any IP Address. */

/** Broadcast IP Address. */
extern NET_ip_addr_t NET_ip_addr_broadcast;

/** Any IP Address. */
extern NET_ip_addr_t NET_ip_addr_any;

/** Macro to set IP_4 address ipaddr to "a.b.c.d" in network byte order. */
#define NET_ip4_addr(ipaddr, a,b,c,d)\
                    (ipaddr)->addr = ACE_htonl(((ACE_u32_t)(a & 0xff) << 24) |\
                                           ((ACE_u32_t)(b & 0xff) << 16) |\
                                           ((ACE_u32_t)(c & 0xff) << 8) |\
                                            (ACE_u32_t)(d & 0xff))

/** Macro to set IP address dest to IP address src.*/
#define NET_ip_addr_set(dest, src) (dest)->addr = \
                                   ((src) == NULL ? NET_IP_ADDR_ANY : \
		                            (src)->addr)

/** Macro to compare 2 IP addresses (each is masked with mask). */
#define NET_ip_addr_maskcmp(addr1, addr2, mask) (((addr1)->addr & \
                                                 (mask)->addr) == \
                                                ((addr2)->addr & \
                                                 (mask)->addr))

/** Macro to compare 2 IP addresses. */
#define NET_ip_addr_cmp(addr1, addr2) ((addr1)->addr == (addr2)->addr)

/** Macro to check if IP address is any. */
#define NET_ip_addr_isany(addr1) ((addr1)->addr == NET_IP_ADDR_ANY )

/** Macro to check if IP address is broadcast. */
#define NET_ip_addr_isbroadcast(addr1, mask) (((((addr1)->addr) & ~((mask)->addr)) == \
			     		                       (0xffffffff & ~((mask)->addr))) || \
                                              ((addr1)->addr == 0xffffffff) || \
                                              ((addr1)->addr == 0x00000000))

/** Macro to check if IP address is multicast. */
#define NET_ip_addr_ismulticast(addr1) (((addr1)->addr & ACE_ntohl(0xf0000000)) == \
										ACE_ntohl(0xe0000000))

/** Macro to get 1 byte of IP address. */
#define NET_ip4_addr1(ipaddr) ((ACE_u8_t)(ACE_ntohl((ipaddr)->addr) >> 24) & 0xff)

/** Macro to get 2 byte of IP address. */
#define NET_ip4_addr2(ipaddr) ((ACE_u8_t)(ACE_ntohl((ipaddr)->addr) >> 16) & 0xff)

/** Macro to get 3 byte of IP address. */
#define NET_ip4_addr3(ipaddr) ((ACE_u8_t)(ACE_ntohl((ipaddr)->addr) >> 8) & 0xff)

/** Macro to get 4 byte of IP address. */
#define NET_ip4_addr4(ipaddr) ((ACE_u8_t)(ACE_ntohl((ipaddr)->addr)) & 0xff)

/** Macro to debug(print) IP address. */
#define NET_ip_addr_debug(ipaddr) DEBUGF(NET_IP_DEBUG, ("%d.%d.%d.%d", \
                                         NET_ip4_addr1(ipaddr), \
                                         NET_ip4_addr2(ipaddr), \
                                         NET_ip4_addr3(ipaddr), \
                                         NET_ip4_addr4(ipaddr) ))

/** @}
 */

#endif
