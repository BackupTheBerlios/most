/*
 * bootp.c
 *
 *  Created on: 20.11.2013
 *      Author: maik
 */

#include <cli/exec.h>
#include <cli/config.h>
#include <nap/bootp.h>

static CLI_exec_t bootp;

extern ACE_err_t
CLI_bootp (void)
{
    if (NAP_bootp (&CLI_config.eth_addr) == 0)
    {
        CLI_config.ip_addr = NAP_bootp_data.ip_addr;
        CLI_config.gateway = NAP_bootp_data.gateway;
        CLI_config.server = NAP_bootp_data.server;
    }
    return ACE_OK;
}

static ACE_err_t
bootp_exec (char *nix)
{
    return CLI_bootp();
}

extern void
CLI_bootp_install (void)
{
    MFS_descriptor_t *dir = MFS_resolve ("/sys/cli/exe");
    CLI_exec_init (dir, &bootp, "bootp", "Bootp request", bootp_exec);
}

