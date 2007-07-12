/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdlib.h>
#include <net/ethif.h>
#include <net/inet.h>
#include <cli/commands.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>

#include "arch/93C46.h"
#include "init/config.h"

/*
 * EE works only with words ! 
 */

#define MAC_ADDR_START     0
#define CHECKSUM_SIZE      2
#define MAC_ADDR_BUF_SIZE  (NET_ETH_ADDR_SIZE + CHECKSUM_SIZE)
#define HOSTNAME_START     (MAC_ADDR_START + MAC_ADDR_BUF_SIZE)
#define HOSTNAME_SIZE      32
#define HOSTNAME_BUF_SIZE  (HOSTNAME_SIZE + CHECKSUM_SIZE)

static struct NET_eth_addr eth0_mac = { {0xA0, 0xBA, 0xD0, 0xCA, 0xFE, 0x01} };

static MFS_descriptor_t *config;

static CLI_exec_t mac;

static void
mac_exec (char *addr)
{
    if (addr != NULL)
    {
        unsigned char mac_addr[MAC_ADDR_BUF_SIZE];
        int i;
        bool_t err = FALSE;
        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
	    int x;
	    x = atoxc(addr);
	    if (x >= 0){
	        mac_addr[i] = x;
	        addr += 2;
	    } else {
                err = TRUE;
                break;
            }
        }
        if (err == FALSE) {
            memcpy (eth0_mac.addr, mac_addr, sizeof (eth0_mac.addr));
            *(u16_t *) (mac_addr + NET_ETH_ADDR_SIZE) =
                NET_inet_chksum (mac_addr, NET_ETH_ADDR_SIZE);
            EE_93C46_write (MAC_ADDR_START, sizeof (mac_addr), mac_addr);
        }
        else
        {
            puts ("\ngive addr as a1b2c3d4f5A6.");
        }
    }
    printf ("\nETH0 addr: %02x %02x %02x %02x %02x %02x",
                eth0_mac.addr[0], eth0_mac.addr[1], eth0_mac.addr[2],
		eth0_mac.addr[3], eth0_mac.addr[4], eth0_mac.addr[5]);
}

/*
 * checksum geht fr ee erased ( alles ff) daneben! 
 */

static void
read_mac (void)
{
    unsigned char mac_addr[MAC_ADDR_BUF_SIZE];
    EE_93C46_read (MAC_ADDR_START, sizeof (mac_addr), mac_addr);
    if (NET_inet_chksum (mac_addr, (u16_t) sizeof (mac_addr)) == 0)
    {
        memcpy (eth0_mac.addr, mac_addr, sizeof (eth0_mac.addr));
    }
}

extern struct NET_eth_addr *
MDC_get_eth0_mac_addr (void)
{
    return &eth0_mac;
}


static char *hostname;

static char hostname_buf[HOSTNAME_SIZE + 1];

static CLI_exec_t host;

static void
host_exec (char *name)
{
    if (name != NULL)
    {
        unsigned char host_name[HOSTNAME_BUF_SIZE];
        int len;
        len = strlen (name);
        if (len <= HOSTNAME_SIZE)
        {
            hostname = hostname_buf;
            memset (host_name, 0, sizeof (host_name));
            memcpy (host_name, name, len);
            memcpy (hostname, host_name, len);
            hostname[len] = '\0';
            *(u16_t *) (host_name + HOSTNAME_SIZE) =
                NET_inet_chksum (host_name, HOSTNAME_SIZE);
            EE_93C46_write (HOSTNAME_START, sizeof (host_name), host_name);
        }
        else
        {
            puts ("\nhostname is limited to 32 characters.");
        }
    }
    printf ("\nHost name: %s", hostname);
}

static void
read_host (void)
{
    unsigned char host_name[HOSTNAME_BUF_SIZE];
    EE_93C46_read (HOSTNAME_START, sizeof (host_name), host_name);
    if (NET_inet_chksum (host_name, (u16_t) sizeof (host_name)) == 0)
    {
        hostname = hostname_buf;
        memcpy (hostname, host_name, HOSTNAME_SIZE);
        hostname[HOSTNAME_SIZE] = '\0';
    }
    else
    {
        hostname = "Nobody";
    }
}


extern const char *
MDC_get_hostname (void)
{
    return hostname;
}

extern void
MDC_config_read (void)
{
    read_mac ();
    read_host ();
}

extern void
MDC_config_install (void)
{
	config = MFS_create_dir(MFS_sysfs_root(), "config");
    CLI_exec_init (config, &mac, "mac", "Show or set mac address", mac_exec);
    CLI_exec_init (config, &host, "host", "Show or set host name", host_exec);
}
