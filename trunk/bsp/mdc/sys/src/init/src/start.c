/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/timer.h>
#include <cli/interpreter.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/commands.h>
#include <nap/bootp.h>
#include <nap/syslog.h>

#include "arch/cpu.h"
#include "arch/digio.h"
#include "arch/ticks.h"
#include "arch/eth.h"
#include "init/start.h"
#include "init/main.h"
#include "init/config.h"
#include "init/bsp_commands.h"

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
    for (;;)
    {
        MDC_watchdog_trigger ();
        DEV_digout_toggle (&MDC_run_led);
        USO_sleep (USO_MSEC_2_TICKS (200));
    }
}

static void
start_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Start.\n");

    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "Run");
    USO_start (&run_led_thread);
    USO_log_puts (USO_LL_INFO, "Run led.\n");
    USO_yield ();

    MDC_eth_start ();
    USO_log_puts (USO_LL_INFO, "Eth on.\n");

    CLI_setup (MDC_config.hostname);
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ACE_ARRAYSIZE (cli0_stack), USO_USER, USO_ROUND_ROBIN, "Cli0");
    USO_thread_arg_init (&cli0_thread, &cli0);

    USO_start (&cli0_thread);
    USO_log_puts (USO_LL_INFO, "Cli0 on.\n");

    if (MDC_config.flags & MDC_CONFIG_FLAG_BOOTP)
        MDC_bootp ();

    /* Network configured */

    NAP_syslog_open (&MDC_config.ip_addr, &MDC_config.server);
    NAP_syslog_puts (NAP_SYSLOG_INFO, NAP_SYSLOG_LOCAL0, "Syslog on");

    MFS_descriptor_t *bsp;
    bsp = MFS_create_dir (MFS_sysfs_get_dir (MFS_SYSFS_DIR_ROOT), "bsp");

    MDC_config_install (bsp);
    MDC_bsp_commands_install (bsp);

    DEV_digout_set (&MDC_green_led);
    MDC_main ();
}

extern void
MDC_start (ACE_FILE * stdio)
{
    USO_log_puts (USO_LL_INFO, "Start.\n");
    USO_thread_t *start_thread = USO_thread_new ((void (*)(void *))start_run,
                                                 START_STACK_SIZE, USO_USER, USO_ROUND_ROBIN,
                                                 "Start");
    USO_thread_ios_init (start_thread, stdio, stdio);
    USO_thread_flags_set (start_thread, 1 << USO_FLAG_DETACH);
    USO_start (start_thread);
}
