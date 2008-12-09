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

#define MDC_CFG_HOSTNAME_SIZE      14
#define MDC_CFG_FILENAME_SIZE	   32

#define MDC_EE_CONFIG_STATE_DEFAULT   1
#define MDC_EE_CONFIG_STATE_SAVED     2
#define MDC_EE_CONFIG_STATE_ALTERED   3

#define MDC_EE_CONFIG_FLAG_BOOTP   0x0001
#define MDC_EE_CONFIG_FLAG_TFTP    0x0002

struct MDC_ee_configuration
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

extern struct MDC_ee_configuration MDC_ee_config;

/**
 * Read config from eeprom.
 */
extern void MDC_ee_config_read (void);

/**
 * Read config from eeprom.
 */
extern void MDC_ee_config_ip(void);

/**
 * Install config commands.
 */
extern void MDC_config_install (void);

/** @}
 */

/** @}
 */

#endif
