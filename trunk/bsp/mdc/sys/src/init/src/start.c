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
#include <cli/exec.h>

#include <arch/eth.h>
#include <arch/pins.h>
#include <arch/digio.h>
#include <arch/ticks.h>
#include <init/start.h>
#include <init/main.h>
#include <init/config.h>
#include <init/bsp_commands.h>

#define TIMERS_STACK_SIZE     (0x200/sizeof(USO_stack_t))
#define RUNLED_STACK_SIZE     (0x200/sizeof(USO_stack_t))
#define CLI_STACK_SIZE        (0x400/sizeof(USO_stack_t))
#define START_STACK_SIZE      (0x400/sizeof(USO_stack_t))

static USO_thread_t run_led_thread;
static USO_stack_t run_led_stack[RUNLED_STACK_SIZE];

static USO_thread_t cli0_thread;
static USO_stack_t cli0_stack[CLI_STACK_SIZE];
static CLI_interpreter_t cli0;

static ACE_err_t
run_led_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Run led is running.\n");
    for (;;)
    {
        MDC_watchdog_trigger ();
        DEV_digout_toggle (&MDC_run_led);
        USO_sleep (USO_MSEC_2_TICKS (100));
    }
    return DEF_ERR_SYS;
}

static ACE_err_t
start_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "is running.\n");

    USO_log_puts (USO_LL_INFO, MDC_BOARD" : "ACE_MOST_BUILD"\n");

    USO_log_puts (USO_LL_INFO, "Timers start.\n");
    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Run led start.\n");
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "run");
    USO_start (&run_led_thread);
    USO_yield();

    USO_log_puts (USO_LL_INFO, "Eth start.\n");
    MDC_eth_start ();

    USO_log_puts (USO_LL_INFO, "Cli0 start.\n");
    CLI_setup (MDC_config.hostname);
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ACE_ARRAYSIZE (cli0_stack), USO_USER, USO_ROUND_ROBIN, "cli0");
    USO_thread_arg_init (&cli0_thread, &cli0);
    USO_start (&cli0_thread);

    MFS_descriptor_t *dir = MFS_resolve("/bsp");
    MDC_bsp_commands_install (dir);
    MDC_config_install (dir);
    MFS_close_desc(dir);

    DEV_digout_set (&MDC_green_led);
    MDC_main ();
    return ACE_OK;
}

extern void
MDC_start (ACE_FILE * stdio)
{
    USO_log_puts (USO_LL_INFO, "Start ");
    USO_thread_t *start_thread = USO_thread_new (start_run,
                                                 START_STACK_SIZE,
                                                 USO_USER,
                                                 USO_ROUND_ROBIN,
                                                 "start");
    USO_thread_in_init (start_thread, stdio);
    USO_thread_out_init (start_thread, stdio);
    USO_thread_flags_set (start_thread, 1 << USO_FLAG_DETACH);
    USO_start (start_thread);
}
