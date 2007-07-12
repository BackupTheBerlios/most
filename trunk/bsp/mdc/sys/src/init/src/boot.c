/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/arch/cpu.h>
#include <cli/commands.h>
#include <nap/bootp.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>

#include "arch/reset.h"
#include "arch/eth.h"
#include "init/boot.h"
#include "init/config.h"

MFS_descriptor_t *boot;

static CLI_exec_t start_boot;
static CLI_exec_t start_app;
static CLI_exec_t bootp;

static void
bootp_exec (char *nix)
{
    NAP_bootp (MDC_get_eth0_mac_addr ());
    NET_netif_set_ipaddr (&MDC_eth0, NAP_bootp_ip_address ());
    NET_netif_set_gateway (&MDC_eth0, NAP_bootp_server_address ());
    //NET_netif_set_netmask (&MDC_eth0, NAP_bootp_subnetmask ());
    USO_kputs (USO_LL_INFO, "NET initialized.\n");
}

static void
start_boot_exec (char *nix)
{
	USO_disable();
	MDC_jump_boot();
}

static void
start_app_exec (char *nix)
{
	USO_disable();
	MDC_jump_app();
}

extern void
MDC_boot_install (void)
{
	boot = MFS_create_dir(MFS_sysfs_root(), "boot");
    CLI_exec_init (boot, &bootp, "bootp", "Bootp request", bootp_exec);
    CLI_exec_init (boot, &start_boot, "startb", "Start boot", start_boot_exec);
    CLI_exec_init (boot, &start_app, "starta", "Start app", start_app_exec);
}
