#ifndef NET_ETHERNET_H
#define NET_ETHERNET_H

#define NET_ETH_TYPE_ARP 0x0806 /**< ARP Protocol */
#define NET_ETH_TYPE_IP  0x0800 /**< IP Protocol */

extern void
NET_eth_input (NET_netif_t * netif,
			   NET_netbuf_t * p);

extern NET_err_t
NET_eth_output (NET_netif_t * netif,
				NET_netbuf_t * p,
				NET_ip_addr_t * ipaddr);

extern void
NET_eth_init (void);

#endif /*NET_ETHERNET_H*/
