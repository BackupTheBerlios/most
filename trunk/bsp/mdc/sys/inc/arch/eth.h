/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_ETH_H
#define MDC_ETH_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup eth eth.h
 *
 * Ethernet.
 *
 * @{
 */


#include <net/netif.h>

/** Loopback device */
extern NET_netif_t MDC_lo;

/** Ethernet device 0 */
extern NET_netif_t MDC_eth0;

/**
 * Initialize ethernet.
 */
extern void MDC_eth_init (void);

/**
 * Start ethernet.
 */
extern void MDC_eth_start (void);

/** @}
 */

/** @}
 */

#endif
