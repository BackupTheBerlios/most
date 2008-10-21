/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/arch/cpu.h>
#include <uso/sleep.h>
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

extern void
MDC_bootp(void)
{
    if (NAP_bootp (&MDC_ee_config.eth_addr) == 0){
    	NET_netif_set_ipaddr (&MDC_eth0, &NAP_bootp_data.ip_addr);
    	NET_netif_set_gateway (&MDC_eth0, &NAP_bootp_data.gateway);
    	MDC_ee_config.ip_addr = NAP_bootp_data.ip_addr;
    	MDC_ee_config.gateway = NAP_bootp_data.gateway;
    	MDC_ee_config.server = NAP_bootp_data.server;
	}
}

static void
bootp_exec (char *nix)
{
	MDC_bootp();
}

extern void
MDC_start_boot(void)
{
	USO_sleep(ACE_MSEC_2_TICKS(200));
	USO_disable();
	MDC_jump_boot();
}

static void
start_boot_exec (char *nix)
{
	MDC_start_boot();
}

extern void
MDC_start_app(void)
{
	USO_sleep(ACE_MSEC_2_TICKS(200));
	USO_disable();
	MDC_jump_app();
}

static void
start_app_exec (char *nix)
{
	MDC_start_app();
}

extern void
MDC_boot_install (void)
{
	boot = MFS_create_dir(MFS_sysfs_root(), "boot");
    CLI_exec_init (boot, &bootp, "bootp", "Bootp request", bootp_exec);
    CLI_exec_init (boot, &start_boot, "startb", "Start boot", start_boot_exec);
    CLI_exec_init (boot, &start_app, "starta", "Start app", start_app_exec);
}
