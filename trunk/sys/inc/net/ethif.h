/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ETHIF_H
#define NET_ETHIF_H

#include <uso/mailbox.h>
#include <uso/thread.h>
#include <uso/buf_pool.h>

#include "net/netif.h"
#include "net/ethdev.h"
#include "net/ethaddr.h"

/** @defgroup ethif ethif.h
 *
 * Ethernet Interface.
 *
 * @{
 */

#define NET_ETH_RX_TX_QUE_SIZE 10 /**< ? */

struct NET_ethif
{
    USO_buf_pool_t rx_pool;
    NET_netbuf_t rx_packets[NET_ETH_RX_TX_QUE_SIZE];
    USO_mailbox_t ll_tx_que;
    USO_thread_t ll_rx_thread;
    USO_stack_t ll_rx_stack[400];
    USO_thread_t ll_tx_thread;
    USO_stack_t ll_tx_stack[400];
    struct NET_eth_addr *ethaddr;
    NET_ethdev_t *ethdev;
};

/** Ethernet Interface */
typedef struct NET_ethif NET_ethif_t;

#define NET_ETH_TYPE_ARP 0x0806 /**< ARP Protocol */
#define NET_ETH_TYPE_IP  0x0800 /**< IP Protocol */

/**
 * Start ethernet interface 
 */
extern void NET_ethif_start (NET_netif_t * netif);

/**
 * Initialize ethernet interface 
 */
extern void NET_ethif_init (NET_ethif_t *,
                            NET_netif_t *,
                            struct NET_eth_addr *hwaddr, NET_ethdev_t * ethdev);

/** @}
 */

#endif
