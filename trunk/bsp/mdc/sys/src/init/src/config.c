/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/commands.h>
#include <mfs/directory.h>

#include "arch/93C46.h"
#include "arch/eth.h"
#include "init/config.h"

struct MDC_ee_configuration MDC_ee_config;

static unsigned char default_eth_addr[NET_ETH_ADDR_SIZE] = {0x00, 0xa0, 0xba, 0xd0, 0xca, 0xfe};

static void ee_config_init(void)
{
	memset (MDC_ee_config.hostname, 0x0, sizeof(MDC_ee_config.hostname));
	ACE_strcpy (MDC_ee_config.hostname, "nobody");
	memcpy (MDC_ee_config.eth_addr.addr, default_eth_addr, sizeof(MDC_ee_config.eth_addr.addr));
	NET_ip4_addr (&MDC_ee_config.ip_addr, 192, 168, 2, 100);
    NET_ip4_addr (&MDC_ee_config.netmask, 255, 255, 255, 0);
    NET_ip4_addr (&MDC_ee_config.gateway, 192, 168, 2, 1);
    NET_ip4_addr (&MDC_ee_config.server, 192, 168, 2, 1);
	memset (MDC_ee_config.filename, 0x0, sizeof(MDC_ee_config.filename));
	ACE_strcpy (MDC_ee_config.filename, "mdc.bin");
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
    EE_93C46_read (0, sizeof (MDC_ee_config), &MDC_ee_config);
    if ( (NET_inet_chksum (&MDC_ee_config,
    	 (ACE_u16_t) (sizeof (MDC_ee_config))) != 0) ||
    	 (MDC_ee_config.state != MDC_EE_CONFIG_STATE_SAVED) ) {
    	ee_config_init();
    }
}

static void
ee_config_write (void)
{
	if (MDC_ee_config.state == MDC_EE_CONFIG_STATE_SAVED)
		return;
	MDC_ee_config.state = MDC_EE_CONFIG_STATE_SAVED;
	MDC_ee_config.checksum = NET_inet_chksum (&MDC_ee_config,
		(ACE_u16_t) (sizeof (MDC_ee_config) - sizeof(MDC_ee_config.checksum)));
	EE_93C46_write (0, sizeof (MDC_ee_config), &MDC_ee_config);	
}

static void
ee_config_print_ip_addr(char * name, ACE_u32_t ip_addr)
{
    ACE_printf ("%s: %3ld.%3ld.%3ld.%3ld\n", name,
             ip_addr >> 24 & 0xff,
             ip_addr >> 16 & 0xff,
             ip_addr >> 8 & 0xff,
             ip_addr & 0xff);
}

static void
ee_config_print (void)
{
	ACE_puts ("EE Config:\n");
    ACE_printf ("hostname: %s\n", MDC_ee_config.hostname);
    ACE_printf ("eth_addr: %02x %02x %02x %02x %02x %02x\n",
        MDC_ee_config.eth_addr.addr[0], MDC_ee_config.eth_addr.addr[1], MDC_ee_config.eth_addr.addr[2],
		MDC_ee_config.eth_addr.addr[3], MDC_ee_config.eth_addr.addr[4], MDC_ee_config.eth_addr.addr[5]);
    ee_config_print_ip_addr("ip_addr", ACE_ntohl(MDC_ee_config.ip_addr.addr));
    ee_config_print_ip_addr("netmask", ACE_ntohl(MDC_ee_config.netmask.addr));
    ee_config_print_ip_addr("gateway", ACE_ntohl(MDC_ee_config.gateway.addr));
    ee_config_print_ip_addr("server ", ACE_ntohl(MDC_ee_config.server.addr));
    ACE_printf ("filename: %s\n", MDC_ee_config.filename);
    ACE_printf ("flags: %04X\n", MDC_ee_config.flags);
    ACE_printf ("state: %u\n", MDC_ee_config.state);
}

extern void
MDC_ee_config_ip(void)
{
	NET_netif_set_ipaddr (&MDC_eth0, &MDC_ee_config.ip_addr);
	NET_netif_set_netmask (&MDC_eth0, &MDC_ee_config.netmask);
	NET_netif_set_gateway (&MDC_eth0, &MDC_ee_config.gateway);
}

static void
ee_config_set_hostname (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, MDC_CFG_HOSTNAME_SIZE - 1);
	    memcpy (MDC_ee_config.hostname, name, len);
        MDC_ee_config.hostname[len] = '\0';
    }
}

static void
ee_config_set_eth_addr (char *addr)
{
    if (addr != NULL)
    {
        unsigned char tmp[NET_ETH_ADDR_SIZE];
        int i;
        ACE_bool_t err = FALSE;
        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
	    	int x;
	    	x = ACE_atoxc(addr);
	    	if (x >= 0){
	        	tmp[i] = x;
	        	addr += 2;
	    	} else {
                err = TRUE;
                break;
            }
        }
        if (err == FALSE) {
            memcpy (MDC_ee_config.eth_addr.addr, tmp, NET_ETH_ADDR_SIZE);
        } else {
            ACE_puts ("give addr as A1A2A3A4A5A6.\n");
        }
    }
}

static void ee_config_set_ip_addr(char* param, NET_ip_addr_t* ipaddr)
{
	if (param != NULL)
	{
		int a,b,c,d;
		param = ACE_strtok(param, ".");
		a = ACE_atoi(param);
		param = ACE_strtok(NULL, ".");
		b = ACE_atoi(param);
		param = ACE_strtok(NULL, ".");
		c = ACE_atoi(param);
		param = ACE_strtok(NULL, ".");
		d = ACE_atoi(param);
		NET_ip4_addr(ipaddr, a,b,c,d);
	}
}

static void
ee_config_set_filename (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, MDC_CFG_FILENAME_SIZE - 1);
	    memcpy (MDC_ee_config.filename, name, len);
        MDC_ee_config.filename[len] = '\0';
    }
}

static void
ee_config_set_flags(char *param)
{
	if (param != NULL)
	{
		int hi, lo;
		hi = ACE_atoxc(param);
		lo = ACE_atoxc(param + 2);
	    if (hi >= 0 && lo >= 0){
			ACE_u16_t flags = hi << 8 || lo;
			MDC_ee_config.flags |= flags;
	    }
	}
}

static void
ee_config_clear_flags(char *param)
{
	if (param != NULL)
	{
		int hi, lo;
		hi = ACE_atoxc(param);
		lo = ACE_atoxc(param + 2);
	    if (hi >= 0 && lo >= 0){
			ACE_u16_t flags = hi << 8 || lo;
			MDC_ee_config.flags &= ~flags;
	    }
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
			case 'i':
				MDC_ee_config_ip();
				break;
			default:
	            ACE_puts ("r read, w write, s show, i conf ip.\n");
				break;
		}
	}
}

static void
set_exec (char *param)
{
	if (param != NULL){
		MDC_ee_config.state = MDC_EE_CONFIG_STATE_ALTERED;
		char a = *param;
		param++;
		switch (a)
		{
			case 'd':
				ee_config_init();
				break;
			case 'h':
				ee_config_set_hostname(param);
				break;
			case 'e':
				ee_config_set_eth_addr(param);
				break;
			case 'i':
				ee_config_set_ip_addr(param, &MDC_ee_config.ip_addr);
				break;
			case 'n':
				ee_config_set_ip_addr(param, &MDC_ee_config.netmask);
				break;
			case 'g':
				ee_config_set_ip_addr(param, &MDC_ee_config.gateway);
				break;
			case 's':
				ee_config_set_ip_addr(param, &MDC_ee_config.server);
				break;
			case 'f':
				ee_config_set_filename(param);
				break;
			case 'z':
				ee_config_set_flags(param);	
				break;
			case 'y':
				ee_config_clear_flags(param);	
				break;
			default:
	            ACE_puts ("d default, h host, e eth_addr, i ip_addr, n netmask,\n"
	            	  "g gateway, s server, f file z set flags y clear flags.\n");
				break;
		}
	}
}

extern void
MDC_config_install (MFS_descriptor_t *bsp)
{
	config = MFS_create_dir(bsp, "config");
    CLI_exec_init (config, &conf, "conf", "ee config", conf_exec);
    CLI_exec_init (config, &set, "set", "set ee config field", set_exec);
}
