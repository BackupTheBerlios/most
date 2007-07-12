/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_SMC_91C94_H
#define NET_SMC_91C94_H

#include "net/ethif.h"
#include "net/ethdev.h"

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup smc_91c94 smc_91c94.h
 *
 * SMC 91c94
 *
 * @{
 */


#define SMC_MEMORY_WAIT_TIME 4 /**< ? */
#define SMC_CHIP_TIMEOUT 1000  /**< ? */

struct NET_smc91c94
{
    NET_ethdev_t ethdev;
    unsigned long io_addr;
    unsigned char mac_addr[NET_ETH_ADDR_SIZE];
    // unsigned long rx_packets;
    unsigned long rx_overruns;
    unsigned long rx_dropped;
    unsigned long rx_error;
    unsigned long rx_badlen;
    unsigned long tx_packets;
    // static unsigned long tx_errors;
    // static unsigned long tx_alloc;
    // static unsigned long tx_alloctries;
    unsigned long tx_allocfailed;
    // static unsigned long tx_allocfatal = 0;
    // static unsigned long tx_ptfovrn;
    // static unsigned long tx_povrn;
    unsigned long ercv_ints;
    // static unsigned long eph_ints;
    // static unsigned long tx_ints;
    // static unsigned long tx_empty;
    unsigned long tx_exc_def;
    unsigned long tx_def;
    unsigned long tx_mcc;
    unsigned long tx_scc;

    unsigned long link_down_count;
    // static unsigned long eth_chip_resets = 0;

    bool_t link_is_up;

    USO_semaphore_t rx_sem;

};

/** SMC91C94 device type. */
typedef struct NET_smc91c94 NET_smc91c94_t;

/** ? */
extern void NET_smc_init (NET_smc91c94_t * smc,
                          struct NET_eth_addr *hwaddr, unsigned long io_addr);

/** @}
 */

/** @}
 */

#endif
