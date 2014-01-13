/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/esc.h>
#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <cli/interpreter.h>
#include <cli/tty.h>
#include <cli/switch.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/exec.h>
#include <dev/mmc.h>
#include <dev/timer.h>

#include <arch/cpu.h>
#include <arch/kbd.h>
#include <init/start.h>
#include <init/main.h>

#define START_STACK_SIZE      (0x2000/sizeof(USO_stack_t))
#define TIMERS_STACK_SIZE     (0x1000/sizeof(USO_stack_t))
#define RUNLED_STACK_SIZE     (0x1000/sizeof(USO_stack_t))
#define CLI_STACK_SIZE        (0x2000/sizeof(USO_stack_t))
#define CLI_RUN_STACK_SIZE    (0x2000/sizeof(USO_stack_t))

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
        USO_sleep (USO_MSEC_2_TICKS (1000));
        USO_log_puts (USO_LL_PROTOCOL, "R");
    }
    return DEF_ERR_SYS;
}

static ACE_err_t
start_run (void *nix)
{
    /* Kernel logging on */
    USO_sleep (USO_MSEC_2_TICKS (3000)); /* to see lowlevel print */
    ACE_puts(ACE_ESC_CLEAR_SCREEN);
    USO_log_puts (USO_LL_INFO, PC_BOARD" : "ACE_MOST_VERSION_STRING"\n");
    USO_log_puts (USO_LL_INFO, "Start: Kernel log on tty0.\n");

    unsigned long ticks_start = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS(10000L)); /* todo DEV_cpudelay does not work ??? Qemu??? */
    USO_log_printf (USO_LL_INFO, "Loop calib 10ms: %lu.\n",
                    USO_TICKS_2_MSEC(DEV_get_ticks_diff (ticks_start)));

    USO_log_puts (USO_LL_INFO, "Timer start.\n");
    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Run LED start.\n");
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "run");
    USO_start (&run_led_thread);
    USO_yield();

    CLI_setup ("pc");

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

    PC_main ();
    return ACE_OK;
}

extern void
PC_start (ACE_FILE * stdio)
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
