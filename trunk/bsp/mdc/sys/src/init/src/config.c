/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#include <ace/err.h>
#include <cli/config.h>

#include <arch/eth.h>
#include <arch/ee.h>
#include <init/config.h>


static ACE_err_t
config_read (void)
{
    EE_93C46_read (&ee, 0, sizeof (CLI_config),(unsigned short *)&CLI_config);
    return ACE_OK;
}

static ACE_err_t
config_write (void)
{
    EE_93C46_write (&ee, 0, sizeof (CLI_config), (unsigned short *)&CLI_config);
    return ACE_OK;
}


static ACE_err_t
config_ip (void)
{
    NET_netif_set_ipaddr (&MDC_eth0, &CLI_config.ip_addr);
    NET_netif_set_netmask (&MDC_eth0, &CLI_config.netmask);
    NET_netif_set_gateway (&MDC_eth0, &CLI_config.gateway);
    return ACE_OK;
}


extern void
MDC_config_create (void)
{
    CLI_config_init ();
    CLI_config_read ();
    CLI_config_install (config_read, config_write, config_ip);
}
