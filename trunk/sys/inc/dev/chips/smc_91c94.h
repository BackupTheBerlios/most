/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_SMC_91C94_H
#define DEV_SMC_91C94_H

#include "net/ethif.h"

/** @addtogroup dev
 *
 * @{
 */

/** @addtogroup chips
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

struct DEV_smc91c94
{
    unsigned long io_addr;
    unsigned char mac_addr[NET_ETH_ADDR_SIZE];
    unsigned long rx_packets;
    unsigned short rx_overruns;
    unsigned short rx_dropped;
    unsigned short rx_error;
    unsigned short rx_badlen;
    unsigned long tx_packets;
    unsigned short tx_errors;
    unsigned short tx_empty;
    unsigned short tx_allocfailed;
    unsigned short ercv_ints;
    unsigned short eph_ints;
    unsigned short alloc_ints;
    unsigned long tx_exc_def;
    unsigned long tx_def;
    unsigned long tx_mcc;
    unsigned long tx_scc;
    unsigned short link_down_count;
    ACE_bool_t link_is_up;
    USO_semaphore_t rx_sem;
};

/** SMC91C94 device type. */
typedef struct DEV_smc91c94 DEV_smc91c94_t;

extern void DEV_smc_interrupt (DEV_smc91c94_t * smc);

/** ? */
extern void DEV_smc_init (NET_ethif_t * ethif, DEV_smc91c94_t * smc, unsigned long io_addr);

/** @}
 */

/** @}
 */

/** @}
 */

#endif
