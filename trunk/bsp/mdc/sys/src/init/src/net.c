/*
 * net.c
 *
 *  Created on: 04.01.2013
 *      Author: maik
 */

#include <cli/exec.h>
#include <mfs/sysfs.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <nap/syslog.h>
#include <init/net.h>
#include <init/config.h>
#include <arch/eth.h>

static CLI_exec_t bootp;

extern void
MDC_bootp (void)
{
    if (NAP_bootp (&MDC_config.eth_addr) == 0)
    {
        NET_netif_set_ipaddr (&MDC_eth0, &NAP_bootp_data.ip_addr);
        NET_netif_set_gateway (&MDC_eth0, &NAP_bootp_data.gateway);
        MDC_config.ip_addr = NAP_bootp_data.ip_addr;
        MDC_config.gateway = NAP_bootp_data.gateway;
        MDC_config.server = NAP_bootp_data.server;
    }
}

static void
bootp_exec (char *nix)
{
    MDC_bootp ();
}

static void
net_basic_install (void)
{
    CLI_exec_init (MFS_resolve(MFS_get_root(), "bsp"), &bootp, "bootp", "Bootp request", bootp_exec);
    NAP_tftp_install(&MDC_config.ip_addr, &MDC_config.server);
}

extern void
MDC_net_start(void (*net_add_install)(void))
{
    if (MDC_config.flags & MDC_CONFIG_FLAG_BOOTP){
        MDC_bootp ();
    }

    if (MDC_config.flags & MDC_CONFIG_FLAG_SYSLOG){
        NAP_syslog_open (&MDC_config.ip_addr, &MDC_config.server);
        NAP_syslog_puts (NAP_SYSLOG_INFO, NAP_SYSLOG_LOCAL0, "Syslog on");
    }

    net_basic_install();
    if (net_add_install != NULL) net_add_install();
    USO_log_puts (USO_LL_INFO, "Net startup done.\n");
}
