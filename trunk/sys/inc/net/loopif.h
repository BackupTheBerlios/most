/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_LOOPIF_H
#define NET_LOOPIF_H

#include "net/netif.h"

/** @defgroup loopif loopif.h
 *
 * Loopback interface.
 *
 * @{
 */

/**
 * Initialize loopback interface.
 * Set netif output function to loopback output.
 * @param netif : Network interface.
 */
extern void NET_loopif_init (NET_netif_t *netif);

/** @}
 */

#endif
