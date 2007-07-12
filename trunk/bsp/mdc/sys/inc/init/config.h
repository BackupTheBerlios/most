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

/**
 * Get the mac address.
 */
extern struct NET_eth_addr *MDC_get_eth0_mac_addr (void);

/**
 * Get the Hostname.
 */
extern const char *MDC_get_hostname (void);

/**
 * Read config from eeprom.
 */
extern void MDC_config_read (void);

/**
 * Install config commands.
 */
extern void MDC_config_install (void);

/** @}
 */

/** @}
 */

#endif
