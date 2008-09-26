/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NAP_BOOTP_H
#define NAP_BOOTP_H

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
    
/**
 * Do a bootp request.
 * @param hwaddr eth mac address.
 */
extern void NAP_bootp (struct NET_eth_addr *hwaddr);

/**
 * @return IP address.
 */
extern NET_ip_addr_t * NAP_bootp_ip_address (void);

/**
 * @return Server IP address.
 */
extern NET_ip_addr_t * NAP_bootp_server (void);

/**
 * @return Server IP address.
 */
extern NET_ip_addr_t * NAP_bootp_gateway (void);

/**
 * @return name of the bootfile.
 */
extern char const *NAP_bootp_filename (void);

/** @}
 */

#endif
