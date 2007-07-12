/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ARP_H
#define NET_ARP_H

#include "net/netbuf.h"
#include "net/ip_addr.h"
#include "net/netif.h"
#include "net/ethif.h"

/** @defgroup arp arp.h
 *
 * Address resolution protocol.
 *
 * @{
 */

/** ? */
#define ARP_TMR_INTERVAL ACE_MSEC_2_TICKS(10000)

/** Initialize. 
 *	
 */
extern void NET_arp_init (void);

/**  
 *  @param netif
 *  @param netbuffer
 */
extern void NET_arp_ip_input (NET_netif_t *netif, NET_netbuf_t *netbuffer);

/**  
 *  @param netif
 *  @param eth_addr
 *  @param netbuffer
 *  @return Pointer to netbuffer
 */
extern NET_netbuf_t *NET_arp_arp_input (NET_netif_t * netif,
                                        struct NET_eth_addr *eth_addr, NET_netbuf_t *netbuffer);

/**  
 *  @param addr Ip Address
 *  @return Ethernet Address
 */
extern struct NET_eth_addr *NET_arp_lookup (NET_ip_addr_t *addr);

/**  
 *  @param netif
 *  @param eth_addr
 *  @param netbuffer
 *  @return Pointer to netbuffer
 */
extern NET_netbuf_t *NET_arp_query (NET_netif_t *netif,
                                    struct NET_eth_addr *eth_addr, NET_ip_addr_t *netbuffer);

/** @}
 */

#endif
