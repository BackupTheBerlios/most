/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/commands.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>

#include "arch/93C46.h"
#include "init/config.h"

struct MDC_ee_configuration MDC_ee_config;

static unsigned char default_mac[NET_ETH_ADDR_SIZE] = {0xA0, 0xBA, 0xD0, 0xCA, 0xFE, 0x01};

static void ee_config_init(void)
{
	strcpy (MDC_ee_config.hostname, "nobody");
	memcpy (MDC_ee_config.mac.addr, default_mac, sizeof(MDC_ee_config.mac.addr));
	NET_ip4_addr (&MDC_ee_config.ip_address, 192, 168, 2, 100);
    NET_ip4_addr (&MDC_ee_config.netmask, 255, 255, 255, 0);
    NET_ip4_addr (&MDC_ee_config.gateway, 192, 168, 2, 1);
    NET_ip4_addr (&MDC_ee_config.server, 192, 168, 2, 1);
	strcpy (MDC_ee_config.filename, "mdc.bin");
	MDC_ee_config.flags = 0;
	MDC_ee_config.state = MDC_EE_CONFIG_STATE_DEFAULT;
	MDC_ee_config.checksum = 0;
}

/*
 * checksum is for ee erased ( all ff) not correct! 
 */

extern void
MDC_ee_config_read (void)
{
	if (MDC_ee_config.state == MDC_EE_CONFIG_STATE_ALTERED)
		return;
    EE_93C46_read (0, sizeof (MDC_ee_config), &MDC_ee_config);
    if ( (NET_inet_chksum (&MDC_ee_config,
    	 (u16_t) (sizeof (MDC_ee_config))) != 0) ||
    	 (MDC_ee_config.state != MDC_EE_CONFIG_STATE_SAVED) ) {
    	ee_config_init();
    }
}

static void
ee_config_write (void)
{
	if (MDC_ee_config.state != MDC_EE_CONFIG_STATE_ALTERED)
		return;
	MDC_ee_config.state = MDC_EE_CONFIG_STATE_SAVED;
	MDC_ee_config.checksum = NET_inet_chksum (&MDC_ee_config,
		(u16_t) (sizeof (MDC_ee_config) - sizeof(MDC_ee_config.checksum)));
	EE_93C46_write (0, sizeof (MDC_ee_config), &MDC_ee_config);	
}

static void
ee_config_print_ip_addr(char * name, u32_t ip_addr)
{
    printf ("%s:  %3ld  |  %3ld  |  %3ld  |  %3ld\n", name,
             ip_addr >> 24 & 0xff,
             ip_addr >> 16 & 0xff,
             ip_addr >> 8 & 0xff,
             ip_addr & 0xff);
}

static void
ee_config_print (void)
{
	puts ("EE Config:\n");
    printf ("hostname: %s\n", MDC_ee_config.hostname);
    printf ("mac: %02x %02x %02x %02x %02x %02x\n",
        MDC_ee_config.mac.addr[0], MDC_ee_config.mac.addr[1], MDC_ee_config.mac.addr[2],
		MDC_ee_config.mac.addr[3], MDC_ee_config.mac.addr[4], MDC_ee_config.mac.addr[5]);
    ee_config_print_ip_addr("ip_addr", ntohl(MDC_ee_config.ip_address.addr));
    ee_config_print_ip_addr("netmask", ntohl(MDC_ee_config.netmask.addr));
    ee_config_print_ip_addr("gateway", ntohl(MDC_ee_config.gateway.addr));
    ee_config_print_ip_addr("server", ntohl(MDC_ee_config.server.addr));
    printf ("filename: %s\n", MDC_ee_config.filename);
    printf ("state: %04X\n", MDC_ee_config.flags);
    printf ("state: %uh\n", MDC_ee_config.state);
}

static void
ee_config_set_hostname (char *name)
{
    if (name != NULL)
    {
        int len;
        len = strnlen (name, MDC_HOSTNAME_SIZE - 1);
	    memcpy (MDC_ee_config.hostname, name, len);
        MDC_ee_config.hostname[len] = '\0';
    }
}

static void
ee_config_set_mac (char *addr)
{
    if (addr != NULL)
    {
        unsigned char tmp[NET_ETH_ADDR_SIZE];
        int i;
        bool_t err = FALSE;
        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
	    	int x;
	    	x = atoxc(addr);
	    	if (x >= 0){
	        	tmp[i] = x;
	        	addr += 2;
	    	} else {
                err = TRUE;
                break;
            }
        }
        if (err == FALSE) {
            memcpy (MDC_ee_config.mac.addr, tmp, NET_ETH_ADDR_SIZE);
        } else {
            puts ("give addr as A1A2A3A4A5A6.\n");
        }
    }
}

static void
ee_config_set_filename (char *name)
{
    if (name != NULL)
    {
        int len;
        len = strnlen (name, MDC_FILENAME_SIZE - 1);
	    memcpy (MDC_ee_config.filename, name, len);
        MDC_ee_config.filename[len] = '\0';
    }
}

static MFS_descriptor_t *config;
static CLI_exec_t conf;
static CLI_exec_t set;

static void
conf_exec (char *param)
{
	if (param != NULL){
		char a = *param;
		param++;
		switch (a)
		{
			case 'r':
				MDC_ee_config_read();
				break;
			case 'w':
				ee_config_write();
				break;
			case 's':
				ee_config_print();
				break;
			default:
	            puts ("r read, w write, s show\n");
				break;
		}
	}
}

static void
set_exec (char *param)
{
	if (param != NULL){
		char a = *param;
		param++;
		switch (a)
		{
			case 'h':
				ee_config_set_hostname(param);
				break;
			case 'm':
				ee_config_set_mac(param);
				break;
			case 'i':
				break;
			case 'n':
				break;
			case 's':
				break;
			case 'f':
				ee_config_set_filename(param);
				break;
			default:
	            puts ("h host, m mac, i ipaddr, n netmask, s server, f file\n");
				break;
		}
	}
}

extern void
MDC_config_install (void)
{
	config = MFS_create_dir(MFS_sysfs_root(), "config");
    CLI_exec_init (config, &conf, "conf", "read, write conf to eeprom or show conf", conf_exec);
    CLI_exec_init (config, &set, "set", "set config member", set_exec);
}
