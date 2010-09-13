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
#include <dev/arch/cpu.h>
#include <cli/interpreter.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/commands.h>

#include "arch/digio.h"
#include "arch/eth.h"
#include "init/init.h"
#include "init/events.h"
#include "init/start.h"
#include "init/main.h"
#include "init/bsp_commands.h"

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
		//SAM_watchdog_trigger ();
        DEV_digout_toggle (&SAM_run_led);
        SAM_events_create();
        USO_sleep (ACE_MSEC_2_TICKS(100));
    }
}

static void
start_run (void *nix)
{
    USO_kputs (USO_LL_INFO, "Start run.\n");

    SAM_events_init();
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack),
                     USO_SYSTEM, USO_FIFO, "Run");
    USO_start (&run_led_thread);
    USO_kputs (USO_LL_INFO, "Run led.\n");
	USO_yield();

	SAM_eth_start();
    USO_kputs (USO_LL_INFO, "Eth on.\n");

    CLI_setup ("samy");
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ACE_ARRAYSIZE (cli0_stack),
                     USO_USER, USO_ROUND_ROBIN, "Cli0");
    USO_thread_arg_init (&cli0_thread, &cli0);
    USO_thread_ios_init (&cli0_thread, ser0, ser0);
    USO_start (&cli0_thread);
    USO_kputs (USO_LL_INFO, "Cli0 on ser0.\n");

	MFS_descriptor_t *bsp;
	bsp = MFS_create_dir(MFS_sysfs_root(), "bsp");

	bsp_commands_install(bsp);

    DEV_digout_set (&SAM_green_led);
    SAM_main();
}

extern void
SAM_start (void)
{
    USO_kputs (USO_LL_INFO, "Start.\n");
   	USO_thread_t *start_thread = USO_thread_new ((void (*)(void*))start_run,
   			START_STACK_SIZE, USO_USER, USO_ROUND_ROBIN, "Start");
    USO_thread_flags_set(start_thread, 1 << USO_FLAG_DETACH);
	USO_start (start_thread);
}
