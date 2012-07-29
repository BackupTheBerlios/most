/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/commands.h>
#include <mfs/directory.h>

#include <arch/spi.h>
#include <arch/eth.h>
#include <init/config.h>

struct MDC_configuration MDC_config;

EE_93C46_t ee;

extern ACE_err_t
MDC_ee_init(void)
{
    return EE_93C46_init(&ee, (DEV_spi_dev_t *)(&MDC_spi_ee));
}

static unsigned char default_eth_addr[NET_ETH_ADDR_SIZE] = { 0x00, 0xa0, 0xba, 0xd0, 0xca, 0xfe };

extern void
MDC_config_init (void)
{
    memset (MDC_config.hostname, 0x0, sizeof (MDC_config.hostname));
    ACE_strcpy (MDC_config.hostname, "nobody");
    memcpy (MDC_config.eth_addr.addr, default_eth_addr, sizeof (MDC_config.eth_addr.addr));
    NET_ip4_addr (&MDC_config.ip_addr, 192, 168, 2, 100);
    NET_ip4_addr (&MDC_config.netmask, 255, 255, 255, 0);
    NET_ip4_addr (&MDC_config.gateway, 192, 168, 2, 1);
    NET_ip4_addr (&MDC_config.server, 192, 168, 2, 1);
    memset (MDC_config.filename, 0x0, sizeof (MDC_config.filename));
    ACE_strcpy (MDC_config.filename, "mdc.bin");
    MDC_config.flags = 0;
    MDC_config.state = MDC_CONFIG_STATE_DEFAULT;
    MDC_config.checksum = 0;
}

/*
 * checksum is for ee erased ( all ff) not correct! 
 */

extern void
MDC_config_read (void)
{
    EE_93C46_read (&ee, 0, sizeof (MDC_config),(unsigned short *)&MDC_config);
    if ((NET_inet_chksum (&MDC_config,
                          (ACE_u16_t) (sizeof (MDC_config))) != 0) ||
        (MDC_config.state != MDC_CONFIG_STATE_SAVED))
    {
        MDC_config_init ();
    }
}

static void
config_write (void)
{
    if (MDC_config.state == MDC_CONFIG_STATE_SAVED)
        return;
    MDC_config.state = MDC_CONFIG_STATE_SAVED;
    MDC_config.checksum = NET_inet_chksum (&MDC_config,
                                              (ACE_u16_t) (sizeof (MDC_config) -
                                                           sizeof (MDC_config.checksum)));
    EE_93C46_write (&ee, 0, sizeof (MDC_config), (unsigned short *)&MDC_config);
}

static void
config_print_ip_addr (char *name, ACE_u32_t ip_addr)
{
    ACE_printf ("%s: %3ld.%3ld.%3ld.%3ld\n", name,
                ip_addr >> 24 & 0xff, ip_addr >> 16 & 0xff, ip_addr >> 8 & 0xff, ip_addr & 0xff);
}

static void
config_print (void)
{
    ACE_puts ("Config:\n");
    ACE_printf ("hostname: %s\n", MDC_config.hostname);
    ACE_printf ("eth_addr: %02x %02x %02x %02x %02x %02x\n",
                MDC_config.eth_addr.addr[0], MDC_config.eth_addr.addr[1],
                MDC_config.eth_addr.addr[2], MDC_config.eth_addr.addr[3],
                MDC_config.eth_addr.addr[4], MDC_config.eth_addr.addr[5]);
    config_print_ip_addr ("ip_addr", ACE_ntohl (MDC_config.ip_addr.addr));
    config_print_ip_addr ("netmask", ACE_ntohl (MDC_config.netmask.addr));
    config_print_ip_addr ("gateway", ACE_ntohl (MDC_config.gateway.addr));
    config_print_ip_addr ("server ", ACE_ntohl (MDC_config.server.addr));
    ACE_printf ("filename: %s\n", MDC_config.filename);
    ACE_printf ("flags: %04X\n", MDC_config.flags);
    ACE_printf ("state: %u\n", MDC_config.state);
}

extern void
MDC_config_ip (void)
{
    NET_netif_set_ipaddr (&MDC_eth0, &MDC_config.ip_addr);
    NET_netif_set_netmask (&MDC_eth0, &MDC_config.netmask);
    NET_netif_set_gateway (&MDC_eth0, &MDC_config.gateway);
}

static void
config_set_hostname (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, MDC_CFG_HOSTNAME_SIZE - 1);
        memcpy (MDC_config.hostname, name, len);
        MDC_config.hostname[len] = '\0';
    }
}

static void
config_set_eth_addr (char *addr)
{
    if (addr != NULL)
    {
        unsigned char tmp[NET_ETH_ADDR_SIZE];
        int i;
        ACE_bool_t err = FALSE;
        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
            int x;
            x = ACE_atoxc (addr);
            if (x >= 0)
            {
                tmp[i] = x;
                addr += 2;
            }
            else
            {
                err = TRUE;
                break;
            }
        }
        if (err == FALSE)
        {
            memcpy (MDC_config.eth_addr.addr, tmp, NET_ETH_ADDR_SIZE);
        }
        else
        {
            ACE_puts ("give addr as A1A2A3A4A5A6.\n");
        }
    }
}

static void
config_set_ip_addr (char *param, NET_ip_addr_t * ipaddr)
{
    if (param != NULL)
    {
        int a, b, c, d;
        param = ACE_strtok (param, ".");
        a = ACE_atoi (param);
        param = ACE_strtok (NULL, ".");
        b = ACE_atoi (param);
        param = ACE_strtok (NULL, ".");
        c = ACE_atoi (param);
        param = ACE_strtok (NULL, ".");
        d = ACE_atoi (param);
        NET_ip4_addr (ipaddr, a, b, c, d);
    }
}

static void
config_set_filename (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, MDC_CFG_FILENAME_SIZE - 1);
        memcpy (MDC_config.filename, name, len);
        MDC_config.filename[len] = '\0';
    }
}

static void
config_set_flags (char *param)
{
    if (param != NULL)
    {
        int hi, lo;
        hi = ACE_atoxc (param);
        lo = ACE_atoxc (param + 2);
        if (hi >= 0 && lo >= 0)
        {
            ACE_u16_t flags = hi << 8 || lo;
            MDC_config.flags |= flags;
        }
    }
}

static void
config_clear_flags (char *param)
{
    if (param != NULL)
    {
        int hi, lo;
        hi = ACE_atoxc (param);
        lo = ACE_atoxc (param + 2);
        if (hi >= 0 && lo >= 0)
        {
            ACE_u16_t flags = hi << 8 || lo;
            MDC_config.flags &= ~flags;
        }
    }
}

static CLI_exec_t conf;
static CLI_exec_t set;

static void
conf_exec (char *param)
{
    if (param != NULL)
    {
        char a = *param;
        param++;
        switch (a)
        {
        case 'r':
            MDC_config_read ();
            break;
        case 'w':
            config_write ();
            break;
        case 's':
            config_print ();
            break;
        case 'i':
            MDC_config_ip ();
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
    if (param != NULL)
    {
        MDC_config.state = MDC_CONFIG_STATE_ALTERED;
        char a = *param;
        param++;
        switch (a)
        {
        case 'd':
            MDC_config_init ();
            break;
        case 'h':
            config_set_hostname (param);
            break;
        case 'e':
            config_set_eth_addr (param);
            break;
        case 'i':
            config_set_ip_addr (param, &MDC_config.ip_addr);
            break;
        case 'n':
            config_set_ip_addr (param, &MDC_config.netmask);
            break;
        case 'g':
            config_set_ip_addr (param, &MDC_config.gateway);
            break;
        case 's':
            config_set_ip_addr (param, &MDC_config.server);
            break;
        case 'f':
            config_set_filename (param);
            break;
        case 'z':
            config_set_flags (param);
            break;
        case 'y':
            config_clear_flags (param);
            break;
        default:
            ACE_puts ("d default, h host, e eth_addr, i ip_addr, n netmask,\n"
                      "g gateway, s server, f file z set flags y clear flags.\n");
            break;
        }
    }
}

extern void
MDC_config_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &conf, "conf", "read, write and use config", conf_exec);
    CLI_exec_init (bsp, &set, "set", "set config field", set_exec);
}
