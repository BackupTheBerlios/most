/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NAP_BOOTP_H
#define NAP_BOOTP_H

/** @addtogroup nap
 * @{
 */

/** @defgroup bootp bootp.h
 *
 * Bootp Protocol for host configuration.
 *
 * @{
 */

#include <net/ethif.h>
#include <net/ip_addr.h>

/******************************************************************************** 
 * Interface
 ********************************************************************************/

#define NAP_BOOTP_FILE_NAME_SIZE   32    /**< Bootp filename size. */

/** Bootp data type. */
typedef struct
{
    NET_ip_addr_t ip_addr;                    /**< IP address for the host. */
    NET_ip_addr_t server;                     /**< IP address of server. */
    NET_ip_addr_t gateway;                    /**< IP address of gateway. */
    char filename[NAP_BOOTP_FILE_NAME_SIZE];  /**< Filename for download with tftp. */
} NAP_bootp_data_t;

/** Bootp data type instance. */
extern NAP_bootp_data_t NAP_bootp_data;

/**
 * Do a bootp request.
 * @param hwaddr : ETH MAC address.
 * @return -1 if the bootp request fails, else 0.
 */
extern int NAP_bootp (struct NET_eth_addr *hwaddr);


/** @}
 */

/** @}
 */

#endif
