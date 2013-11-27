/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#include <ace/esc.h>
#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <dev/mmc.h>
#include <dev/timer.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/interpreter.h>
#include <cli/exec.h>
#include <cli/config.h>
#include <cli/switch.h>

#include <arch/cpu.h>
#include <arch/digio.h>
#include <arch/button.h>
#include <init/events.h>
#include <init/start.h>
#include <init/bsp_commands.h>
#include <init/main.h>

#define START_STACK_SIZE      (0x800/sizeof(USO_stack_t))
#define TIMERS_STACK_SIZE     (0x400/sizeof(USO_stack_t))
#define RUNLED_STACK_SIZE     (0x400/sizeof(USO_stack_t))
#define CLI_STACK_SIZE        (0x800/sizeof(USO_stack_t))
#define CLI_RUN_STACK_SIZE    (0x800/sizeof(USO_stack_t))

static MFS_descriptor_t *tty1 = NULL;
static MFS_descriptor_t *tty2 = NULL;
static MFS_descriptor_t *tty3 = NULL;

static USO_thread_t run_led_thread;
static USO_stack_t run_led_stack[RUNLED_STACK_SIZE];

static ACE_err_t
run_led_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Run LED running.\n");
    for (;;)
    {
        DEV_digout_toggle (&LA2_run_led);
        LA2_events_generate ();
        USO_sleep (USO_MSEC_2_TICKS (100));
    }
    return DEF_ERR_SYS;
}

static ACE_err_t
start_run (void *nix)
{
    /* Kernel logging on */
    ACE_puts(ACE_ESC_CLEAR_SCREEN);
    USO_log_puts (USO_LL_INFO, LA2_BOARD" : "ACE_MOST_BUILD"\n");
    USO_log_puts (USO_LL_INFO, "Start: Kernel log on tty0.\n");

    unsigned long ticks_count = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS (100000L));
    USO_log_printf (USO_LL_INFO, "Loop calib 100ms: %lu.\n", USO_TICKS_2_MSEC(DEV_get_ticks_diff (ticks_count)) );

    USO_log_puts (USO_LL_INFO, "Timer start.\n");
    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Run LED start.\n");
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "run");
    USO_start (&run_led_thread);
    USO_yield();

    LA2_button_start();

    CLI_setup (CLI_config.hostname);

    tty1 = MFS_resolve("/sys/cli/tty1");
    if (tty1 == NULL)
    {
        ACE_PANIC ("Open tty1 fail");
    }
    CLI_switch_set(1,tty1);

    tty2 = MFS_resolve("/sys/cli/tty2");
    if (tty2 == NULL)
    {
        ACE_PANIC ("Open tty2 fail");
    }
    CLI_switch_set(2,tty2);

    tty3 = MFS_resolve("/sys/cli/tty3");
    if (tty3 == NULL)
    {
        ACE_PANIC ("Open tty3 fail");
    }
    CLI_switch_set(3,tty3);

    USO_log_puts (USO_LL_INFO, "Cli1 start on F2.\n");
    CLI_interpreter_start ("F2", tty1, CLI_STACK_SIZE, CLI_RUN_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Cli2 start on F3.\n");
    CLI_interpreter_start ("F3", tty2, CLI_STACK_SIZE, CLI_RUN_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Cli3 start on F4.\n");
    CLI_interpreter_start ("F4", tty3, CLI_STACK_SIZE, CLI_RUN_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Cli0 start on F1.\n");
    CLI_interpreter_start ("F1", NULL, CLI_STACK_SIZE, CLI_RUN_STACK_SIZE);

    MFS_descriptor_t *dir;
     dir = MFS_resolve("/bsp");
    LA2_bsp_commands_install (dir);
    MFS_close_desc (dir);

    DEV_digout_set (&LA2_green_led);
    LA2_main ();
    return ACE_OK;
}

extern void
LA2_start (ACE_FILE * stdio)
{
    /* here we are running in ctx of idle thread
     * so no printf or log is allowed!!
     * we just start the start thread!!!
     * interrupts must be enabled!!
     * the USO_start function gives immediately control
     * to the start thread and the idle thread will be continued
     * if no other thread is ready to run!
     */

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
