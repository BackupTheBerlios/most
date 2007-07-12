/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ETHDEV_H
#define NET_ETHDEV_H

#include "net/ethaddr.h"

/** @defgroup ethdev ethdev.h
 *
 * Ethernet Device.
 *
 * @{
 */


/** ETH device type. */
typedef struct NET_ethdev NET_ethdev_t;

/**
 * Ethernet Device
 */
struct NET_ethdev
{
    void (*start) (NET_ethdev_t *); /**< callback */
    void (*interrupt) (NET_ethdev_t *); /**< callback */
    void (*transmit_packet) (NET_ethdev_t *, NET_netbuf_t *); /**< callback */
    NET_netbuf_t *(*receive_packet) (NET_ethdev_t *, USO_buf_pool_t *); /**< callback */
};

/** @}
 */

#endif
