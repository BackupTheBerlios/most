/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ARP_H
#define NET_ARP_H

#include "uso/sleep.h"
#include "net/netbuf.h"
#include "net/ip_addr.h"
#include "net/netif.h"
#include "net/ethif.h"

/** @addtogroup net
 * @{
 */

/** @defgroup arp arp.h
 *
 * Address resolution protocol.
 *
 * @{
 */

/** ARP timer interval.
 * The arp_tmr() function should be called every ARP_TMR_INTERVAL
 * microseconds (10 seconds). This function is responsible for expiring
 * old entries in the ARP table.
 */
#define ARP_TMR_INTERVAL USO_MSEC_2_TICKS(10000)

/** Initialize the ARP table and start timer.
 *	
 */
extern void NET_arp_init (void);

/**  
 * Should be called for all incoming packets of IP kind. The function does
 * not alter the packet in any way, it just updates the ARP table. After
 * this function has been called, the normal TCP/IP stack input function
 * should be called.
 *
 *  @param netif : Network interface of the incoming packet.
 *  @param netbuffer : The incoming IP packet.
 */
extern void NET_arp_ip_input (NET_netif_t * netif, NET_netbuf_t * netbuffer);

/**  
 * Should be called for incoming ARP packets. The netbuffer in the argument is
 * freed by this function. If the function returns a netbuffer (i.e., returns
 * non-NULL), that netbuffer constitutes an ARP reply and should be sent out on
 * the Ethernet.
 *
 *  @param netif : Network interface of the incoming packet.
 *  @param eth_addr : The ethernet address.
 *  @param netbuffer : The received ARP packet.
 *  @return Pointer to netbuffer, if not NULL it is a ARP packet which has to be send.
 */
extern NET_netbuf_t *NET_arp_arp_input (NET_netif_t * netif,
                                        struct NET_eth_addr *eth_addr, NET_netbuf_t * netbuffer);

/**  
 * Arp_lookup() is called to do an IP address -> Ethernet address
 * translation. If the function returns NULL, there is no mapping and the
 * arp_query() function should be called.
 *
 *  @param addr : IP address.
 *  @return Ethernet address or NULL if no mapping.
 */
extern struct NET_eth_addr *NET_arp_lookup (NET_ip_addr_t * addr);

/**  
 * Constructs an ARP query packet for the given IP address. The function
 * returns a netbuf that contains the reply and that should be sent out on
 * the Ethernet.
 *
 *  @param netif : Network interface for the outgoing packet.
 *  @param ethadd : Local ethernet address.
 *  @param ipaddr : Destination IP address of the outgoing packet.
 *  @return Pointer to netbuffer containing the ARP query.
 */
extern NET_netbuf_t *NET_arp_query (NET_netif_t * netif,
                                    struct NET_eth_addr *ethaddr, NET_ip_addr_t * ipaddr);

/** @}
 */

/** @}
 */

#endif
