#ifndef NET_ETHERNET_H
#define NET_ETHERNET_H

/** @addtogroup net
 * @{
 */

/** @defgroup ethernet ethernet.h
 *
 * Ethernet protocol layer.
 *
 * @{
 */

#define NET_ETH_TYPE_ARP 0x0806 /**< ARP Protocol */
#define NET_ETH_TYPE_IP  0x0800 /**< IP Protocol */

/**
 * Incoming ethernet packet.
 * Remove ethernet header (increment netbuf index ) and pass
 * packet to IP layer.
 * Also handle ARP.
 *
 * @param netif : Network interface.
 * @param p : An incoming ethernet packet.
 */
extern void NET_eth_input (NET_netif_t * netif, NET_netbuf_t * p);


/**
 * Outgoing ethernet packet.
 * Add the ethernet header and send the packet on the ethernet interface.
 * Also handles the ARP and gives the netbuffer free.
 *
 * @param netif : Network interface.
 * @param p : Outgoing IP packet.
 * @param ipaddr : Destination IP address.
 * @return NET_ERR_ARP, NET_ERR_MEM or NET_ERR_OK.
 */
extern ACE_err_t NET_eth_output (NET_netif_t * netif, NET_netbuf_t * p, NET_ip_addr_t * ipaddr);

/**
 * Initialize the ethernet layer.
 * Initialize ARP.
 */
extern void NET_eth_init (void);

/** @}
 */

/** @}
 */

#endif /*NET_ETHERNET_H */
