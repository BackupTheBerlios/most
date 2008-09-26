/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_CONFIG_H
#define MDC_CONFIG_H

/** @addtogroup initialization
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

#include <net/ethif.h>
#include <net/inet.h>
#include <ace/stddef.h>

#define MDC_HOSTNAME_SIZE      14
#define MDC_FILENAME_SIZE	   32

#define MDC_EE_CONFIG_STATE_DEFAULT   1
#define MDC_EE_CONFIG_STATE_SAVED     2
#define MDC_EE_CONFIG_STATE_ALTERED   3


struct MDC_ee_configuration
{
	char hostname[MDC_HOSTNAME_SIZE];
	struct NET_eth_addr mac;
	NET_ip_addr_t ip_address;
	NET_ip_addr_t netmask;
	NET_ip_addr_t gateway;
	NET_ip_addr_t server;
	char filename[MDC_FILENAME_SIZE];
	u16_t flags;
	u16_t state;
	u16_t checksum;
} _PACKED_;

extern struct MDC_ee_configuration MDC_ee_config;

/**
 * Read config from eeprom.
 */
extern void MDC_ee_config_read (void);

/**
 * Install config commands.
 */
extern void MDC_config_install (void);

/** @}
 */

/** @}
 */

#endif
