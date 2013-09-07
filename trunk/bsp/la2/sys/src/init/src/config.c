/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/exec.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <mfs/block.h>
#include <dev/mmc.h>

#include <arch/spi.h>
//#include <arch/eth.h>
#include <init/config.h>

struct LA2_configuration LA2_config;

static unsigned char default_eth_addr[NET_ETH_ADDR_SIZE] = { 0x00, 0xa0, 0xba, 0xd0, 0xca, 0xfe };

extern void
LA2_config_init (void)
{
    memset (LA2_config.hostname, 0x0, sizeof (LA2_config.hostname));
    ACE_strcpy (LA2_config.hostname, "nobody");
    memcpy (LA2_config.eth_addr.addr, default_eth_addr, sizeof (LA2_config.eth_addr.addr));
    NET_ip4_addr (&LA2_config.ip_addr, 192, 168, 1, 1);
    NET_ip4_addr (&LA2_config.netmask, 255, 255, 0, 0);
    NET_ip4_addr (&LA2_config.gateway, 192, 168, 1, 100);
    NET_ip4_addr (&LA2_config.server, 192, 168, 1, 100);
    memset (LA2_config.filename, 0x0, sizeof (LA2_config.filename));
    ACE_strcpy (LA2_config.filename, "la2.bin");
    LA2_config.flags = 0;
    LA2_config.state = LA2_CONFIG_STATE_DEFAULT;
    LA2_config.checksum = 0;
}

extern void
LA2_config_read (void)
{
#if 1
    ACE_ssize_t len;
    char *data;
    MFS_descriptor_t *desc = MFS_open (MFS_resolve(MFS_get_root(), "bsp/mmc"), "cfg");
    MFS_block_t *p1 = (MFS_block_t *)desc;
    if (desc == NULL || desc->type != MFS_BLOCK)
        return;
    unsigned long block = p1->start;
    len = MFS_get (p1, &data, block);
    if (data != NULL)
    {
        if (len >= sizeof (LA2_config)){
            memcpy (&LA2_config, data, sizeof (LA2_config));
            if ((NET_inet_chksum (&LA2_config,
                            (ACE_u16_t) (sizeof (LA2_config))) != 0) ||
                (LA2_config.state != LA2_CONFIG_STATE_SAVED))
            {
                LA2_config_init ();
            }
        } else {
            // check error to short data;
        }
        MFS_confirm(p1,block);
    }
    else
    {
        // check error no data
    }
    MFS_close_desc (desc);
#endif
}

static void
config_write (void)
{
#if 1
    MFS_descriptor_t *desc = MFS_open (MFS_resolve(MFS_get_root(), "bsp/mmc"), "cfg");
    MFS_block_t *p1 = (MFS_block_t *)desc;
    if (desc == NULL || desc->type != MFS_BLOCK)
        return;
    if (LA2_config.state == LA2_CONFIG_STATE_SAVED)
        return;
    LA2_config.state = LA2_CONFIG_STATE_SAVED;
    LA2_config.checksum = NET_inet_chksum (&LA2_config,
                                           (ACE_u16_t) (sizeof (LA2_config) -
                                                        sizeof (LA2_config.checksum)));
    char *buffer = ACE_malloc (p1->size);
    if (buffer)
    {
        memset (buffer, '\0', p1->size);
        if (sizeof (LA2_config) <= p1->size)
        {
            memcpy (buffer, &LA2_config, sizeof (LA2_config));
        }
        MFS_put  (p1, buffer, p1->size, p1->start);
        ACE_free (buffer);
    }
    else
    {
        // error out of mem
    }
    MFS_close_desc (desc);
#endif
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
    ACE_printf ("hostname: %s\n", LA2_config.hostname);
    ACE_printf ("eth_addr: %02x %02x %02x %02x %02x %02x\n",
                LA2_config.eth_addr.addr[0], LA2_config.eth_addr.addr[1],
                LA2_config.eth_addr.addr[2], LA2_config.eth_addr.addr[3],
                LA2_config.eth_addr.addr[4], LA2_config.eth_addr.addr[5]);
    config_print_ip_addr ("ip_addr", ACE_ntohl (LA2_config.ip_addr.addr));
    config_print_ip_addr ("netmask", ACE_ntohl (LA2_config.netmask.addr));
    config_print_ip_addr ("gateway", ACE_ntohl (LA2_config.gateway.addr));
    config_print_ip_addr ("server ", ACE_ntohl (LA2_config.server.addr));
    ACE_printf ("filename: %s\n", LA2_config.filename);
    ACE_printf ("flags: %04X (0x0001 = BOOTP, 0x0002 = TFTP, 0x0004 = SYSLOG)\n", LA2_config.flags);
    ACE_printf ("state: %u (1 = default, 2 = saved, 3 = altered)\n", LA2_config.state);
}

extern void
LA2_config_ip (void)
{
#if 0
    NET_netif_set_ipaddr (&SAM_eth0, &SAM_config.ip_addr);
    NET_netif_set_netmask (&SAM_eth0, &SAM_config.netmask);
    NET_netif_set_gateway (&SAM_eth0, &SAM_config.gateway);
#endif
}

static void
config_set_hostname (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, LA2_CFG_HOSTNAME_SIZE - 1);
        memcpy (LA2_config.hostname, name, len);
        LA2_config.hostname[len] = '\0';
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
            memcpy (LA2_config.eth_addr.addr, tmp, NET_ETH_ADDR_SIZE);
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
        USO_lock (&ACE_lock);
        param = ACE_strtok (param, ".");
        a = ACE_atoi (param);
        param = ACE_strtok (NULL, ".");
        b = ACE_atoi (param);
        param = ACE_strtok (NULL, ".");
        c = ACE_atoi (param);
        param = ACE_strtok (NULL, ".");
        d = ACE_atoi (param);
        USO_unlock (&ACE_lock);
        NET_ip4_addr (ipaddr, a, b, c, d);
    }
}

static void
config_set_filename (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, LA2_CFG_FILENAME_SIZE - 1);
        memcpy (LA2_config.filename, name, len);
        LA2_config.filename[len] = '\0';
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
            LA2_config.flags |= flags;
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
            LA2_config.flags &= ~flags;
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
            LA2_config_read ();
            break;
        case 'w':
            config_write ();
            break;
        case 's':
            config_print ();
            break;
        case 'i':
            LA2_config_ip ();
            break;
        default:
            ACE_puts ("inval param, type <conf> for help.\n");
            break;
        }
    } else {
        ACE_puts ("r (read), w (write), s (show), i (conf ip-stack).\n");
    }
}

static void
set_exec (char *param)
{
    if (param != NULL)
    {
        LA2_config.state = LA2_CONFIG_STATE_ALTERED;
        char a = *param;
        param++;
        switch (a)
        {
        case 'd':
            LA2_config_init ();
            break;
        case 'h':
            config_set_hostname (param);
            break;
        case 'e':
            config_set_eth_addr (param);
            break;
        case 'i':
            config_set_ip_addr (param, &LA2_config.ip_addr);
            break;
        case 'n':
            config_set_ip_addr (param, &LA2_config.netmask);
            break;
        case 'g':
            config_set_ip_addr (param, &LA2_config.gateway);
            break;
        case 's':
            config_set_ip_addr (param, &LA2_config.server);
            break;
        case 'f':
            config_set_filename (param);
            break;
        case 'y':
            config_set_flags (param);
            break;
        case 'z':
            config_clear_flags (param);
            break;
        default:
            ACE_puts ("invalid param, type <set> for help.\n");
            break;
        }
    } else {
        ACE_puts ("d (default), h<hostname>, e<eth_addr>, i<ip_addr>, n<netmask>,\n"
                  "g<gateway>, s<server>, f<file>, y<set flags>, z<clear flags>.\n");
    }
}

extern void
LA2_config_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &conf, "conf", "read, write and use config", conf_exec);
    CLI_exec_init (bsp, &set, "set", "set config field", set_exec);
}
