/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_CONFIG_H
#define MDC_CONFIG_H

#include <ace/err.h>
#include <net/ethif.h>
#include <net/inet.h>
#include <ace/stddef.h>
#include <mfs/sysfs.h>
#include <dev/chips/ee_93C46.h>


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

/*
 * EE works only with words ! 
 */


/**
 *
 */
#define MDC_CFG_HOSTNAME_SIZE      14
/**
 *
 */
#define MDC_CFG_FILENAME_SIZE	   32

/**
 *
 */
#define MDC_CONFIG_STATE_DEFAULT   1
/**
 *
 */
#define MDC_CONFIG_STATE_SAVED     2
/**
 *
 */
#define MDC_CONFIG_STATE_ALTERED   3

#define MDC_CONFIG_FLAG_BOOTP     0x0001
#define MDC_CONFIG_FLAG_TFTP      0x0002
#define MDC_CONFIG_FLAG_SYSLOG    0x0004

/**
 * serial ee prom 1Kbit
 */
extern EE_93C46_t ee;

/**
 * MDC configuration
 */
struct MDC_configuration
{
    char hostname[MDC_CFG_HOSTNAME_SIZE];
    struct NET_eth_addr eth_addr;
    NET_ip_addr_t ip_addr;
    NET_ip_addr_t netmask;
    NET_ip_addr_t gateway;
    NET_ip_addr_t server;
    char filename[MDC_CFG_FILENAME_SIZE];
    ACE_u16_t flags;
    ACE_u16_t state;
    ACE_u16_t checksum;
} ACE_PACKED_;

/**
 *
 */
extern struct MDC_configuration MDC_config;

/**
 * Initialize eeprom communication.
 */
extern ACE_err_t MDC_ee_init(void);

/**
 * Initialize config with default values.
 */
extern void MDC_config_init (void);

/**
 * Read config from eeprom.
 */
extern void MDC_config_read (void);

/**
 * Write config to ip stack.
 */
extern void MDC_config_ip (void);

/**
 * Install config commands.
 */
extern void MDC_config_install (MFS_descriptor_t * bsp);

/** @}
 */

/** @}
 */

#endif
