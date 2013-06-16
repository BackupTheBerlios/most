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
#include <cli/exec.h>
#include <dev/mmc.h>
#include <dev/timer.h>

#include <arch/cpu.h>
#include <arch/digio.h>
#include <arch/button.h>
#include <init/events.h>
#include <init/start.h>
#include <init/config.h>
#include <init/bsp_commands.h>
#include <init/main.h>

#define TIMERS_STACK_SIZE     (0x400/sizeof(USO_stack_t))
#define RUNLED_STACK_SIZE     (0x400/sizeof(USO_stack_t))
#define CLI_STACK_SIZE        (0x800/sizeof(USO_stack_t))
#define START_STACK_SIZE      (0x800/sizeof(USO_stack_t))

static USO_thread_t run_led_thread;
static USO_stack_t run_led_stack[RUNLED_STACK_SIZE];

static USO_thread_t cli0_thread;
static USO_stack_t cli0_stack[CLI_STACK_SIZE];

static CLI_interpreter_t cli0;

static void
run_led_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Run led is running.\n");
    for (;;)
    {
#if 0
    	if (!SAM_WDT_DISABLE)
        {
            SAM_wdt_trigger (); /* trigger watchdog only if it is not disabled (WDD) */
        }
#endif
        DEV_digout_toggle (&LA2_run_led);
        LA2_events_generate ();
        USO_sleep (USO_MSEC_2_TICKS (100));
    }
}

static void
start_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "is running.\n");

    USO_log_puts (USO_LL_INFO, LA2_BOARD" : "ACE_MOST_BUILD"\n");

    USO_log_puts (USO_LL_INFO, "Timers start.\n");
    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Run led start.\n");
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "run");
    USO_start (&run_led_thread);
    USO_yield();

    LA2_button_start();

    /* todo eth start has to be done here otherwise in hangs ??? */
    //USO_log_puts (USO_LL_INFO, "Eth start.\n");
    //SAM_eth_start ();

    USO_log_puts (USO_LL_INFO, "Cli0 start.\n");
    CLI_setup (LA2_config.hostname);
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ACE_ARRAYSIZE (cli0_stack), USO_USER, USO_ROUND_ROBIN, "cli0");
    USO_thread_arg_init (&cli0_thread, &cli0);
    USO_start (&cli0_thread);

    LA2_bsp_commands_install (MFS_resolve(MFS_get_root(), "bsp"));
    LA2_config_install (MFS_resolve(MFS_get_root(), "bsp"));

    DEV_digout_set (&LA2_green_led);
    LA2_main ();
}

extern void
LA2_start (ACE_FILE * stdio)
{
    USO_log_puts (USO_LL_INFO, "Start ");
    USO_thread_t *start_thread = USO_thread_new ((void (*)(void *))start_run,
                                                 START_STACK_SIZE,
                                                 USO_USER,
                                                 USO_ROUND_ROBIN,
                                                 "start");
    USO_thread_in_init (start_thread, stdio);
    USO_thread_out_init (start_thread, stdio);
    USO_thread_flags_set (start_thread, 1 << USO_FLAG_DETACH);
    USO_start (start_thread);
}
