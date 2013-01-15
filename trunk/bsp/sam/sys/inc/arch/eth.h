/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_ETH_H
#define SAM_ETH_H

#include <ace/stddef.h>
#include <net/netif.h>

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



/** Loopback device */
extern NET_netif_t SAM_lo;

/** Ethernet device 0 */
extern NET_netif_t SAM_eth0;

/**
 * Initialize ethernet.
 */
extern void SAM_eth_init (void);

/**
 * Start ethernet.
 */
extern void SAM_eth_start (void);

extern void SAM_eth_interrupt (void) ACE_SECTION_ (".unref");

/** @}
 */

/** @}
 */

#endif
