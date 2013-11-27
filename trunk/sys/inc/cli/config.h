/*
 * config.h
 *
 *  Created on: 20.11.2013
 *      Author: maik
 */

#ifndef CLI_CONFIG_H_
#define CLI_CONFIG_H_

#include <ace/err.h>
#include <ace/stddef.h>
#include <net/ethif.h>
#include <net/inet.h>
#include <mfs/sysfs.h>


/** @addtogroup cli
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
 *
 */
#define CLI_CFG_HOSTNAME_SIZE      10
/**
 *
 */
#define CLI_CFG_FILENAME_SIZE      24

enum CLI_config_state
{
    CLI_CFG_STATE_DEFAULT = 1,
    CLI_CFG_STATE_SAVED,
    CLI_CFG_STATE_ALTERED
};


#define CLI_CONFIG_FLAG_BOOTP     0x0001
#define CLI_CONFIG_FLAG_TFTP      0x0002
#define CLI_CONFIG_FLAG_SYSLOG    0x0004

/**
 * CLI configuration
 * max 128 bytes so that it can be stored in a serial eeprom
 */
struct CLI_configuration
{
    char hostname[CLI_CFG_HOSTNAME_SIZE];        // 10
    struct NET_eth_addr eth_addr;                //  6
    NET_ip_addr_t ip_addr;                       //  4
    NET_ip_addr_t netmask;                       //  4
    NET_ip_addr_t gateway;                       //  4
    NET_ip_addr_t server;                        //  4
    char filename[CLI_CFG_FILENAME_SIZE];        // 24
    ACE_u16_t flags;                             //  2
    ACE_u16_t state;                             //  2
    ACE_u16_t checksum;                          //  2
} ACE_PACKED_;

/**
 *  Config as global data
 *
 */
extern struct CLI_configuration     CLI_config;

/**
 * Initialize config with default values.
 */
extern void CLI_config_init (void);

/**
 * Read config from hw.
 */
extern void CLI_config_read (void);

/**
 * Write config to hw.
 */
extern void CLI_config_write (void);

/**
 * Write config to ip stack.
 */
extern void CLI_config_ip (void);

extern void CLI_config_print (void);

extern void CLI_config_set_hostname (char *name);
extern ACE_err_t CLI_config_set_eth_addr (char *addr);
extern void CLI_config_set_ip_addr (char *addr, NET_ip_addr_t * type);
extern void CLI_config_set_filename (char *name);

/**
 * Install config commands.
 */
extern void CLI_config_install (ACE_err_t (*read)(void), ACE_err_t (*write)(void), ACE_err_t (*ip)(void));


#endif /* CONFIG_H_ */

