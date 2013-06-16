/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_CONFIG_H
#define LA2_CONFIG_H

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

#define LA2_CFG_HOSTNAME_SIZE      14
#define LA2_CFG_FILENAME_SIZE	   32

#define LA2_CONFIG_STATE_DEFAULT   1
#define LA2_CONFIG_STATE_SAVED     2
#define LA2_CONFIG_STATE_ALTERED   3

#define LA2_CONFIG_FLAG_BOOTP     0x0001
#define LA2_CONFIG_FLAG_TFTP      0x0002
#define LA2_CONFIG_FLAG_SYSLOG    0x0004

struct LA2_configuration
{
    char hostname[LA2_CFG_HOSTNAME_SIZE];
    struct NET_eth_addr eth_addr;
    NET_ip_addr_t ip_addr;
    NET_ip_addr_t netmask;
    NET_ip_addr_t gateway;
    NET_ip_addr_t server;
    char filename[LA2_CFG_FILENAME_SIZE];
    ACE_u16_t flags;
    ACE_u16_t state;
    ACE_u16_t checksum;
} ACE_PACKED_;

extern struct LA2_configuration LA2_config;

/**
 * Set config to default values.
 */
extern void LA2_config_init (void);


/**
 * Read config.
 */
extern void LA2_config_read (void);

/**
 * Configure IP.
 */
extern void LA2_config_ip (void);

/**
 * Install config commands.
 */
extern void LA2_config_install (MFS_descriptor_t * bsp);

/** @}
 */

/** @}
 */

#endif
