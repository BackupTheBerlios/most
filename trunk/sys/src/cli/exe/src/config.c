/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/exec.h>
#include <cli/arg.h>
#include <cli/config.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/block.h>
#include <dev/mmc.h>


struct CLI_configuration   CLI_config;
static ACE_err_t (*cfg_read)(void) = NULL;
static ACE_err_t (*cfg_write)(void) = NULL;
static ACE_err_t (*cfg_ip)(void) = NULL;

static unsigned char default_eth_addr[NET_ETH_ADDR_SIZE] = { 0xa0, 0xba, 0xd0, 0xca, 0xfe, 0x00 };

extern void
CLI_config_init (void)
{
    ACE_strncpy (CLI_config.hostname, "nobody", sizeof (CLI_config.hostname));
    memcpy (CLI_config.eth_addr.addr, default_eth_addr, sizeof (CLI_config.eth_addr.addr));
    NET_ip4_addr (&CLI_config.ip_addr, 192, 168, 0, 1);
    NET_ip4_addr (&CLI_config.netmask, 255, 255, 255, 0);
    NET_ip4_addr (&CLI_config.gateway, 192, 168, 0, 100);
    NET_ip4_addr (&CLI_config.server, 192, 168, 0, 100);
    ACE_strncpy (CLI_config.filename, "most.bin", sizeof (CLI_config.filename));
    CLI_config.flags = 0;
    CLI_config.state = CLI_CFG_STATE_DEFAULT;
    CLI_config.checksum = 0;
}

/*
 * checksum is for erased (eeprom, flash ...) ( all ff) not correct!
 */
extern void
CLI_config_read (void)
{
    if (cfg_read != NULL){
        if (cfg_read() == ACE_OK){
            if ((NET_inet_chksum (&CLI_config,
                                  (ACE_u16_t) (sizeof (CLI_config))) != 0) ||
                                  (CLI_config.state != CLI_CFG_STATE_SAVED))
            {
                CLI_config_init ();
            }
        } else {
            CLI_config_init ();
        }
    }
}

extern void
CLI_config_write (void)
{
    if (CLI_config.state == CLI_CFG_STATE_SAVED)
        return;
    CLI_config.state = CLI_CFG_STATE_SAVED;
    CLI_config.checksum = NET_inet_chksum (&CLI_config,
                                           (ACE_u16_t) (sizeof (CLI_config) -
                                                        sizeof (CLI_config.checksum)));
    if (cfg_write != NULL){
        cfg_write();
    }
}

extern void
CLI_config_ip (void)
{
    if (cfg_ip != NULL){
        cfg_ip();
    }
}

static void
print_ip_addr (char *name, ACE_u32_t ip_addr)
{
    ACE_printf ("%s: %3ld.%3ld.%3ld.%3ld\n", name,
                ip_addr >> 24 & 0xff, ip_addr >> 16 & 0xff, ip_addr >> 8 & 0xff, ip_addr & 0xff);
}

extern void
CLI_config_print (void)
{
    ACE_puts ("Config:\n");
    ACE_printf ("hostname: %s\n", CLI_config.hostname);
    ACE_printf ("eth_addr: %02x %02x %02x %02x %02x %02x\n",
                CLI_config.eth_addr.addr[0], CLI_config.eth_addr.addr[1],
                CLI_config.eth_addr.addr[2], CLI_config.eth_addr.addr[3],
                CLI_config.eth_addr.addr[4], CLI_config.eth_addr.addr[5]);
    print_ip_addr ("ip_addr", ACE_ntohl (CLI_config.ip_addr.addr));
    print_ip_addr ("netmask", ACE_ntohl (CLI_config.netmask.addr));
    print_ip_addr ("gateway", ACE_ntohl (CLI_config.gateway.addr));
    print_ip_addr ("server ", ACE_ntohl (CLI_config.server.addr));
    ACE_printf ("filename: %s\n", CLI_config.filename);
    ACE_printf ("flags: %04X (0x0001 = BOOTP, 0x0002 = TFTP, 0x0004 = SYSLOG)\n", CLI_config.flags);
    ACE_printf ("state: %u (1 = default, 2 = saved, 3 = altered)\n", CLI_config.state);
}

extern void
CLI_config_set_hostname (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, CLI_CFG_HOSTNAME_SIZE - 1);
        memcpy (CLI_config.hostname, name, len);
        CLI_config.hostname[len] = '\0';
    }
}

extern ACE_err_t
CLI_config_set_eth_addr (char *addr)
{
    ACE_err_t err = ACE_OK;
    if (addr != NULL)
    {
        unsigned char tmp[NET_ETH_ADDR_SIZE];
        int i;
        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
            int x;
            x = ACE_xxtoi (addr);
            if (x >= 0)
            {
                tmp[i] = x;
                addr += 2;
            }
            else
            {
                err = DEF_ERR_ARG;
                break;
            }
        }
        if (err == ACE_OK)
        {
            memcpy (CLI_config.eth_addr.addr, tmp, sizeof (CLI_config.eth_addr.addr));
        }
    }
    return err;
}

extern void
CLI_config_set_ip_addr (char *addr, NET_ip_addr_t * type)
{
    if (addr != NULL)
    {
        int a, b, c, d;
        USO_lock (&ACE_lock);
        addr = ACE_strtok (addr, ".");
        a = ACE_atoi (addr);
        addr = ACE_strtok (NULL, ".");
        b = ACE_atoi (addr);
        addr = ACE_strtok (NULL, ".");
        c = ACE_atoi (addr);
        addr = ACE_strtok (NULL, ".");
        d = ACE_atoi (addr);
        USO_unlock (&ACE_lock);
        NET_ip4_addr (type, a, b, c, d);
    }
}

extern void
CLI_config_set_filename (char *name)
{
    if (name != NULL)
    {
        int len;
        len = ACE_strnlen (name, CLI_CFG_FILENAME_SIZE - 1);
        memcpy (CLI_config.filename, name, len);
        CLI_config.filename[len] = '\0';
    }
}

static void
config_set_flags (char *param)
{
    if (param != NULL)
    {
        int hi, lo;
        hi = ACE_xxtoi (param);
        lo = ACE_xxtoi (param + 2);
        if (hi >= 0 && lo >= 0)
        {
            ACE_u16_t flags = hi << 8 || lo;
            CLI_config.flags |= flags;
        }
    }
}

static void
config_clear_flags (char *param)
{
    if (param != NULL)
    {
        int hi, lo;
        hi = ACE_xxtoi (param);
        lo = ACE_xxtoi (param + 2);
        if (hi >= 0 && lo >= 0)
        {
            ACE_u16_t flags = hi << 8 || lo;
            CLI_config.flags &= ~flags;
        }
    }
}

static CLI_exec_t conf;
static CLI_exec_t set;

static ACE_err_t
conf_exec (char *arg)
{
    ACE_err_t err = ACE_OK;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(arg, argv);
    if (argc >= 1){
        switch (argv[0][0])
        {
            case 'd':
                CLI_config_init ();
                break;
            case 'r':
                CLI_config_read ();
                break;
            case 'w':
                CLI_config_write ();
                break;
            case 's':
                CLI_config_print ();
                break;
            case 'i':
                CLI_config_ip ();
                break;
            case '?':
                ACE_puts ("d (default) r (read), w (write), s (show), i (conf ip-stack)\n");
                break;
            default:
                err = DEF_ERR_ARG;
                break;
        }
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}

static ACE_err_t
set_exec (char *arg)
{
    ACE_err_t err = ACE_OK;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(arg, argv);
    if (argc == 1)
    {
        CLI_config.state = CLI_CFG_STATE_ALTERED;
        switch (argv[0][0])
        {
        case '?':
            ACE_puts ("d (default) | h <hostname> | e <eth_addr> | i <ip addr> | n <netmask> |\n"
                      "g <gateway> | s <server> | f <file> | y <set_flags> | z <clear_flags>.\n");
            break;
        default:
            err = DEF_ERR_ARG;
            break;
        }
    }
    else if (argc == 2)
    {
        CLI_config.state = CLI_CFG_STATE_ALTERED;
        switch (argv[0][0])
        {
        case 'h':
            CLI_config_set_hostname (argv[1]);
            break;
        case 'e':
            err = CLI_config_set_eth_addr (argv[1]);
            break;
        case 'i':
            CLI_config_set_ip_addr (argv[1], &CLI_config.ip_addr);
            break;
        case 'n':
            CLI_config_set_ip_addr (argv[1], &CLI_config.netmask);
            break;
        case 'g':
            CLI_config_set_ip_addr (argv[1], &CLI_config.gateway);
            break;
        case 's':
            CLI_config_set_ip_addr (argv[1], &CLI_config.server);
            break;
        case 'f':
            CLI_config_set_filename (argv[1]);
            break;
        case 'y':
            config_set_flags (argv[1]);
            break;
        case 'z':
            config_clear_flags (argv[1]);
            break;
        default:
            err = DEF_ERR_ARG;
            break;
        }
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}

extern void
CLI_config_install (ACE_err_t (*read)(void), ACE_err_t (*write)(void), ACE_err_t (*ip)(void))
{
    cfg_read = read;
    cfg_write = write;
    cfg_ip = ip;
    MFS_descriptor_t *dir = MFS_resolve ("/sys/cli/exe");
    CLI_exec_init (dir, &conf, "conf", "read, write and show config", conf_exec);
    CLI_exec_init (dir, &set, "set", "set config field", set_exec);
}

