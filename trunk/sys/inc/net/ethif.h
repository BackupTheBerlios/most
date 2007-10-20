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

/** @defgroup ethif ethif.h
 *
 * Ethernet Interface.
 *
 * @{
 */

struct NET_ethif
{
    struct NET_eth_addr *eth_addr;
    void *mac;
    void (*start) (void*);
    NET_netbuf_t* (*receive) (void *);
    void (*transmit) (void *, NET_netbuf_t *);
    void (*info) (void*);
	USO_thread_t rx_thread;
	USO_stack_t rx_stack[400];
};

/** Ethernet Interface */
typedef struct NET_ethif NET_ethif_t;

/**
 * Initialize ethernet interface 
 */
extern void NET_ethif_init (NET_netif_t *netif,
							NET_ethif_t *ethif,
                            struct NET_eth_addr *hwaddr);


/**
 * Start ethernet interface 
 */
extern void NET_ethif_start (NET_ethif_t *ethif);

/** @}
 */

#endif
