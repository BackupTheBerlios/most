/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/cpu.h>
#include <uso/sleep.h>
#include <cli/exec.h>
#include <nap/bootp.h>
#include <mfs/directory.h>

#include <arch/pins.h>
#include <arch/reset.h>
#include <arch/eth.h>
#include <init/bsp_commands.h>
#include <init/config.h>


static CLI_exec_t start_boot;
static CLI_exec_t start_app;

extern void
MDC_start_boot (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    MDC_watchdog_trigger ();
    MDC_jump_boot ();
}

static ACE_err_t
start_boot_exec (char *nix)
{
    MDC_start_boot ();
    return CLI_ERR_NOT_REACHED;
}

extern void
MDC_start_app (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    MDC_watchdog_trigger ();
    MDC_jump_app ();
}

static ACE_err_t
start_app_exec (char *nix)
{
    MDC_start_app ();
    return CLI_ERR_NOT_REACHED;
}



extern void
MDC_bsp_commands_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &start_boot, "start_B", "Start boot", start_boot_exec);
    CLI_exec_init (bsp, &start_app, "start_A", "Start app", start_app_exec);
}
