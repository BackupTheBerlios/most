/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_CONFIG_H
#define SAM_CONFIG_H

#include <mfs/sysfs.h>

/** @addtogroup init
 *
 * @{
 */

/** @defgroup config config.h
 *
 * Configuration
 *
 * @{
 */

#include <net/ethif.h>
#include <net/inet.h>
#include <ace/stddef.h>

#define SAM_CFG_HOSTNAME_SIZE      14
#define SAM_CFG_FILENAME_SIZE	   32

#define SAM_CONFIG_STATE_DEFAULT   1
#define SAM_CONFIG_STATE_SAVED     2
#define SAM_CONFIG_STATE_ALTERED   3

#define SAM_CONFIG_FLAG_BOOTP   0x0001

struct SAM_configuration
{
    char hostname[SAM_CFG_HOSTNAME_SIZE];
    struct NET_eth_addr eth_addr;
    NET_ip_addr_t ip_addr;
    NET_ip_addr_t netmask;
    NET_ip_addr_t gateway;
    NET_ip_addr_t server;
    char filename[SAM_CFG_FILENAME_SIZE];
    ACE_u16_t flags;
    ACE_u16_t state;
    ACE_u16_t checksum;
} ACE_PACKED_;

extern struct SAM_configuration SAM_config;

/**
 * Set config to default values.
 */
extern void SAM_config_init (void);


/**
 * Read config.
 */
extern void SAM_config_read (void);

/**
 * Configure IP.
 */
extern void SAM_config_ip (void);

/**
 * Install config commands.
 */
extern void SAM_config_install (MFS_descriptor_t * bsp);

/** @}
 */

/** @}
 */

#endif
