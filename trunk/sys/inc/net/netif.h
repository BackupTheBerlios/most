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

/** Net Interface */
typedef struct NET_netif NET_netif_t;

/** ? */
struct NET_netif
{
    unsigned int tx;                   /**< Transmitted packets. */
    unsigned int rx;                   /**< Received packets. */
    NET_ip_addr_t ip_addr;
    NET_ip_addr_t netmask;      /* netmask in network byte order */
    NET_ip_addr_t gateway;
    NET_err_t (*output) (NET_netif_t *,NET_netbuf_t *, NET_ip_addr_t * ipaddr);
    void *device;
};

/** ? */
extern NET_netif_t *NET_netif_default;


/**
 * ?
 */
extern void NET_netif_init (NET_netif_t *, char *);

/**
 * ?
 */
extern void NET_netif_set_default (NET_netif_t *);

/**
 * ?
 */
extern void NET_netif_set_ipaddr (NET_netif_t *, NET_ip_addr_t *);

/**
 * ?
 */
extern void NET_netif_set_netmask (NET_netif_t *, NET_ip_addr_t *);

/**
 * ?
 */
extern void NET_netif_set_gateway (NET_netif_t *, NET_ip_addr_t *);

/** @}
 */

#endif
