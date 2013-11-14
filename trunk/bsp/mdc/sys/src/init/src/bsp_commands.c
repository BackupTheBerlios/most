/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/cpu.h>
#include <uso/sleep.h>
#include <cli/exec.h>
#include <nap/bootp.h>
#include <mfs/directory.h>

#include "arch/pins.h"
#include "arch/reset.h"
#include "arch/eth.h"
#include "init/bsp_commands.h"
#include "init/config.h"


static CLI_exec_t start_boot;
static CLI_exec_t start_app;
static CLI_exec_t heap_debug;

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


static ACE_err_t
heap_debug_exec (char *arg)
{
    ACE_err_t err = ACE_OK;
    if (arg != NULL){
        MFS_descriptor_t *desc = MFS_walk_in (MFS_resolve("/sys/uso/heap"), arg, TRUE);
        if (desc != NULL) {
            USO_debug_heap_list (desc->represent);
        } else {
            err = CLI_ERR_NOT_FOUND;
        }
        MFS_close_desc(desc);
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}

extern void
MDC_bsp_commands_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &start_boot, "start_B", "Start boot", start_boot_exec);
    CLI_exec_init (bsp, &start_app, "start_A", "Start app", start_app_exec);
    CLI_exec_init (bsp, &heap_debug, "heap_D", "Debug heap list", heap_debug_exec);
}
