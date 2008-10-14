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

#define NAP_BOOTP_FILE_NAME_SIZE   32

typedef struct 
{
    NET_ip_addr_t ip_addr;
    NET_ip_addr_t server;
    NET_ip_addr_t gateway;
    char filename[NAP_BOOTP_FILE_NAME_SIZE];
}NAP_bootp_data_t;

extern NAP_bootp_data_t NAP_bootp_data;
    
/**
 * Do a bootp request.
 * @param hwaddr eth mac address.
 */
extern int NAP_bootp (struct NET_eth_addr *hwaddr);


/** @}
 */

#endif
