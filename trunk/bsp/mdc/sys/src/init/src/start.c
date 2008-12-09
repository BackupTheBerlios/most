/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <cli/interpreter.h>

#include "arch/cpu.h"
#include "arch/digio.h"
#include "arch/ticks.h"
#include "arch/eth.h"
#include "init/init.h"
#include "init/start.h"
#include "init/main.h"
#include "init/config.h"
#include "init/boot.h"

#define RUNLED_STACK_SIZE     0x50
#define CLI_STACK_SIZE        0x400

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
        USO_sleep (ACE_MSEC_2_TICKS(200));
    }
}

extern void
MDC_start_run (void *nix)
{
    USO_kputs (USO_LL_INFO, "Init run.\n");

    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack),
                     USO_SYSTEM, USO_FIFO, "RunLed");
    USO_start (&run_led_thread);
    USO_kputs (USO_LL_INFO, "RunLed started.\n");
	USO_yield();

	MDC_eth_start();

    CLI_setup (MDC_ee_config.hostname);
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ACE_ARRAYSIZE (cli0_stack),
                      USO_USER, USO_ROUND_ROBIN, "Cli0");
    USO_thread_arg_init (&cli0_thread, &cli0);
    USO_thread_ios_init (&cli0_thread, ser0, ser0);
    USO_start (&cli0_thread);
    USO_kputs (USO_LL_INFO, "Cli0 on ser0.\n");

	if (MDC_ee_config.flags & MDC_EE_CONFIG_FLAG_BOOTP)
		MDC_bootp ();
		
    MDC_config_install ();
	MDC_boot_install ();

    USO_kputs (USO_LL_INFO, "Main.\n");
    DEV_digout_set (&MDC_green_led);
    MDC_main ();
}
