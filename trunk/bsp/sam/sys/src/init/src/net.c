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
#include <arch/exceptions.h>


static CLI_exec_t bootp;

static void
SAM_bootp (void)
{
    if (NAP_bootp (&SAM_config.eth_addr) == 0)
    {
        NET_netif_set_ipaddr (&SAM_eth0, &NAP_bootp_data.ip_addr);
        NET_netif_set_gateway (&SAM_eth0, &NAP_bootp_data.gateway);
        SAM_config.ip_addr = NAP_bootp_data.ip_addr;
        SAM_config.gateway = NAP_bootp_data.gateway;
        SAM_config.server = NAP_bootp_data.server;
    }
}

static ACE_err_t
bootp_exec (char *nix)
{
    SAM_bootp ();
    return ACE_OK;
}

static void
net_basic_install (void)
{
    MFS_descriptor_t *dir =  MFS_resolve("/bsp");
    CLI_exec_init (dir, &bootp, "bootp", "Bootp request", bootp_exec);
    MFS_close_desc(dir);
    NAP_tftp_install(&SAM_config.ip_addr, &SAM_config.server);
}

extern void
SAM_net_start(void (*net_add_install)(void))
{
	if (SAM_config.flags & SAM_CONFIG_FLAG_BOOTP){
        SAM_bootp ();
        USO_log_puts (USO_LL_INFO, "Net config done.\n");
	}

	if (SAM_config.flags & SAM_CONFIG_FLAG_SYSLOG){
		NAP_syslog_open (&SAM_config.ip_addr, &SAM_config.server);
		NAP_syslog_puts (NAP_SYSLOG_INFO, NAP_SYSLOG_LOCAL0, "Syslog on");
	}

    net_basic_install();
    if (net_add_install != NULL) net_add_install();
    USO_log_puts (USO_LL_INFO, "Net startup done.\n");
}
