/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ETHIF_H
#define NET_ETHIF_H

#include <uso/mailbox.h>
#include <uso/thread.h>

#include "net/netif.h"
#include "net/ethaddr.h"

/** @addtogroup net
 * @{
 */

/** @defgroup ethif ethif.h
 *
 * Ethernet Interface.
 *
 * @{
 */

#define ETH_RX_STACK_SIZE     (0x400/sizeof(USO_stack_t))

/** Ethernet Interface structure. */
struct NET_ethif
{
    struct NET_eth_addr *eth_addr;           /**< Ethernet address. */
    void *mac;                               /**< Pointer to MAC device. */
    void (*start) (void *);                  /**< MAC device start function . */
    NET_netbuf_t *(*receive) (void *);       /**< MAC device receive function. */
    void (*transmit) (void *, NET_netbuf_t *);     /**< MAC device transmit function. */
    void (*info) (void *);                   /**< MAC device print information and statistic. */
    USO_thread_t rx_thread;                   /**< Receive thread. */
    USO_stack_t rx_stack[ETH_RX_STACK_SIZE];  /**< Stack for receive thread. */
};

/** Ethernet Interface */
typedef struct NET_ethif NET_ethif_t;

/**
 * Initialize ethernet interface.
 * The ethernet interface is registert as device in the Network interface.
 *
 * @param netif : Network interface.
 * @param ethif : Ethernet interface.
 * @param addr : Ethernet address of the ethernet interface.
 */
extern void NET_ethif_init (NET_netif_t * netif,
                            NET_ethif_t * ethif, struct NET_eth_addr *addr, char *name);


/**
 * Start ethernet interface.
 *
 * @param ethif : Ethernet interface.
 */
extern void NET_ethif_start (NET_ethif_t * ethif);

/** @}
 */

/** @}
 */

#endif
