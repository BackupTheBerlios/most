/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/arch/cpu.h>
#include <uso/sleep.h>
#include <cli/commands.h>
#include <nap/bootp.h>
#include <mfs/directory.h>

#include "arch/cpu.h"
#include "arch/reset.h"
#include "arch/eth.h"
#include "init/bsp_commands.h"
#include "init/config.h"


static CLI_exec_t start_boot;
static CLI_exec_t start_app;
static CLI_exec_t bootp;
static CLI_exec_t heap_debug;

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

extern void
MDC_start_boot (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    MDC_watchdog_trigger ();
    MDC_jump_boot ();
}

static void
start_boot_exec (char *nix)
{
    MDC_start_boot ();
}

extern void
MDC_start_app (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    MDC_watchdog_trigger ();
    MDC_jump_app ();
}

static void
start_app_exec (char *nix)
{
    MDC_start_app ();
}


static void
heap_debug_exec (char *heap)
{
    MFS_descriptor_t *desc = MFS_open (MFS_sysfs_get_dir (MFS_SYSFS_DIR_HEAPS), heap);
    if (desc != NULL)
        USO_debug_heap_list (desc->entry);
}

extern void
MDC_bsp_commands_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &bootp, "bootp", "Bootp request", bootp_exec);
    CLI_exec_init (bsp, &start_boot, "startb", "Start boot", start_boot_exec);
    CLI_exec_init (bsp, &start_app, "starta", "Start app", start_app_exec);
    CLI_exec_init (bsp, &heap_debug, "heap_D", "Debug heap list", heap_debug_exec);
}
