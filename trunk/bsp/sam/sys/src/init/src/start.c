/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <cli/interpreter.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/commands.h>
#include <dev/mmc.h>
#include <dev/timer.h>
#include <nap/bootp.h>
#include <nap/syslog.h>

#include "arch/cpu.h"
#include "arch/spi.h"
#include "arch/digio.h"
#include "arch/eth.h"
#include "init/events.h"
#include "init/start.h"
#include "init/main.h"
#include "init/bsp_commands.h"
#include "init/config.h"

#define TIMERS_STACK_SIZE     (0x200/sizeof(USO_stack_t))
#define RUNLED_STACK_SIZE     (0x200/sizeof(USO_stack_t))
#define CLI_STACK_SIZE        (0x400/sizeof(USO_stack_t))
#define START_STACK_SIZE      (0x400/sizeof(USO_stack_t))

static USO_thread_t run_led_thread;
static USO_stack_t run_led_stack[RUNLED_STACK_SIZE];

static USO_thread_t cli0_thread;
static USO_stack_t cli0_stack[CLI_STACK_SIZE];

static CLI_interpreter_t cli0;

static void
run_led_run (void *nix)
{
    if (!SAM_WDT_DISABLE)
    {
        SAM_wdt_setup (SAM_WDV_2SEC);   /* initial wtv is 16 sec */
    }
    for (;;)
    {
        if (!SAM_WDT_DISABLE)
        {
            SAM_wdt_trigger ();
        }
        DEV_digout_toggle (&SAM_run_led);
        SAM_events_create ();
        USO_sleep (USO_MSEC_2_TICKS (100));
    }
}

static void
start_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "is running.\n");

    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "run");
    USO_start (&run_led_thread);
    USO_log_puts (USO_LL_INFO, "Run led.\n");
    USO_yield ();

    SAM_eth_start ();
    USO_log_puts (USO_LL_INFO, "Eth on.\n");

    CLI_setup (SAM_config.hostname);
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ACE_ARRAYSIZE (cli0_stack), USO_USER, USO_ROUND_ROBIN, "cli0");
    USO_thread_arg_init (&cli0_thread, &cli0);
    USO_start (&cli0_thread);
    USO_log_puts (USO_LL_INFO, "Cli0 on.\n");

    if (SAM_config.flags & SAM_CONFIG_FLAG_BOOTP)
        SAM_bootp ();

    /* Network configured */

    NAP_syslog_open (&SAM_config.ip_addr, &SAM_config.server);
    NAP_syslog_puts (NAP_SYSLOG_INFO, NAP_SYSLOG_LOCAL0, "Syslog on");

    MFS_descriptor_t *bsp;
    bsp = MFS_create_dir (MFS_sysfs_get_dir (MFS_SYSFS_DIR_ROOT), "bsp");

    SAM_bsp_commands_install (bsp);
    SAM_config_install (bsp);

    DEV_digout_set (&SAM_green_led);
    SAM_main ();
}

extern void
SAM_start (ACE_FILE * stdio)
{
    USO_log_puts (USO_LL_INFO, "Start ");
    USO_thread_t *start_thread = USO_thread_new ((void (*)(void *))start_run,
                                                 START_STACK_SIZE,
                                                 USO_USER,
                                                 USO_ROUND_ROBIN,
                                                 "start");
    USO_thread_ios_init (start_thread, stdio, stdio);
    USO_thread_flags_set (start_thread, 1 << USO_FLAG_DETACH);
    USO_start (start_thread);
}
