/*
 * net.c
 *
 *  Created on: 04.01.2013
 *      Author: maik
 */

#include <cli/exec.h>
#include <cli/config.h>
#include <nap/syslog.h>
#include <init/net.h>
#include <init/config.h>
#include <arch/eth.h>
#include <arch/exceptions.h>



static void
net_basic_install (void)
{
    CLI_bootp_install(); /* can be done after CLI_bootp (); */
    CLI_tftp_install(&CLI_config.ip_addr, &CLI_config.server);
}

extern void
SAM_net_start(void (*net_add_install)(void))
{
    SAM_eth_init ();    /* must be called after SAM_config_read() */
    USO_log_puts (USO_LL_INFO, "Eth start.\n");
    SAM_eth_start ();

    /* first do bootp to have right configuration */
    if (CLI_config.flags & CLI_CONFIG_FLAG_BOOTP){
        CLI_bootp ();
        CLI_config_ip ();
    }

    if (CLI_config.flags & CLI_CONFIG_FLAG_SYSLOG){
        NAP_syslog_open (&CLI_config.ip_addr, &CLI_config.server);
        NAP_syslog_puts (NAP_SYSLOG_INFO, NAP_SYSLOG_LOCAL0, "Syslog on");
    }

    net_basic_install();
    if (net_add_install != NULL) net_add_install();

    USO_log_puts (USO_LL_INFO, "Net startup done.\n");
}
