/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_NETIF_H
#define NET_NETIF_H

#include "net/netbuf.h"
#include "net/err.h"
#include "net/ip_addr.h"

/** @defgroup netif netif.h
 *
 * Network Interface.
 *
 * @{
 */

/** Network interface type.*/
typedef struct NET_netif NET_netif_t;

/** Network interface structure. */
struct NET_netif
{
    unsigned long tx;                   /**< Transmitted packets. */
    unsigned long rx;                   /**< Received packets. */
    unsigned short tx_drop;             /**< Transmitted packets dropped. */
    unsigned short rx_drop;             /**< Received packets dropped. */
    NET_ip_addr_t ip_addr;              /**< IP address of the interface. */
    NET_ip_addr_t netmask;              /**< Netmask of the interface. */
    NET_ip_addr_t gateway;              /**< IP address for the gateway. */
    NET_err_t (*output) (NET_netif_t *,NET_netbuf_t *, NET_ip_addr_t * ipaddr); /**< Output function the interface will use. */
    void (*info)(void*);                /**< Info function of the device. */
    void *device;                       /**< Pointer to device driver( ethernet interface, loopback interface, ...). */
};

/** Default network interface. */
extern NET_netif_t *NET_netif_default;


/**
 * Initialize network interface and register it in the filesystem.
 *
 * @param netif : Network interface.
 * @param name : Name for the network interface.
 */
extern void NET_netif_init (NET_netif_t *netif, char *name);

/**
 * Set default network interface to netif.
 */
extern void NET_netif_set_default (NET_netif_t *netif);

/**
 * Set IP address to ipaddr.
 */
extern void NET_netif_set_ipaddr (NET_netif_t *netif, NET_ip_addr_t *ipaddr);

/**
 * Set IP netmask to netmask.
 */
extern void NET_netif_set_netmask (NET_netif_t *netif, NET_ip_addr_t *netmask);

/**
 * Set gateway IP address to gateway.
 */
extern void NET_netif_set_gateway (NET_netif_t *netif, NET_ip_addr_t *gateway);

/** @}
 */

#endif
