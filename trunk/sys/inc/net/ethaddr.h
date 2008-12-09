/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ETHADDR_H
#define NET_ETHADDR_H

#include <ace/stddef.h>

/** @defgroup ethaddr ethaddr.h
 *
 * Ethernet Address.
 *
 * @{
 */

 /** Size of the ethernet address */
#define NET_ETH_ADDR_SIZE 6

/** Ethernet address type. */
struct NET_eth_addr
{
    ACE_u8_t addr[NET_ETH_ADDR_SIZE]; /**< Ethernet address */
} ACE_PACKED_;

/** Ethernet Protocol Header. */
struct NET_eth_hdr
{
    struct NET_eth_addr dest; /**< Destination Address */
    struct NET_eth_addr src;  /**< Source Address */
    ACE_u16_t type;               /**< Protocoll Type */
} ACE_PACKED_;

/** @}
 */

#endif
