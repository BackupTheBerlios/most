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
#include <dev/arch/at91sam7x/rst.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/exec.h>
#include <cli/interpreter.h>
#include <cli/config.h>
#include <cli/switch.h>

#include <arch/cpu.h>
#include <arch/spi.h>
#include <arch/digio.h>
#include <arch/eth.h>
#include <init/events.h>
#include <init/start.h>
#include <init/main.h>
#include <init/bsp_commands.h>

#define START_STACK_SIZE      (0x400/sizeof(USO_stack_t))
#define TIMERS_STACK_SIZE     (0x200/sizeof(USO_stack_t))
#define RUNLED_STACK_SIZE     (0x200/sizeof(USO_stack_t))
#define CLI_STACK_SIZE        (0x400/sizeof(USO_stack_t))
#define CLI_RUN_STACK_SIZE    (0x400/sizeof(USO_stack_t))

static MFS_descriptor_t *tty1 = NULL;

static USO_thread_t run_led_thread;
static USO_stack_t run_led_stack[RUNLED_STACK_SIZE];

static ACE_err_t
run_led_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Run LED running.\n");
    int c = 0;
    for (;;)
    {
        if (!SAM_WDT_DISABLE)
        {
            SAM_wdt_trigger (); /* trigger watchdog only if it is not disabled (WDD) */
        }
        /* todo, user reset is not done automatically (even if it is configured
         * it is recoqnized but not executed)?????, so check for it !!!
         * is my board defect ?
         * a user reset occurs sporadic without pressing button
         * dont exe user reset!
         * board is broken!!!
         */
        if (DEV_at91_RST_is_user_reset_detected (AT91C_BASE_RSTC))
        {
            USO_log_printf (USO_LL_ERROR, "user reset detected, 0x%08lx %s!\n", DEV_at91_RST_status (AT91C_BASE_RSTC),
                            DEV_at91_RST_is_busy (AT91C_BASE_RSTC) ? "busy": "not busy");
            c++;
            USO_sleep (USO_MSEC_2_TICKS (300));
            if (c >= 3){
                c = 0;
                DEV_at91_RST_peripheral_reset (AT91C_BASE_RSTC);
                DEV_at91_RST_processor_reset (AT91C_BASE_RSTC);
            }
        }
        DEV_digout_toggle (&SAM_run_led);
        SAM_events_create ();
        USO_sleep (USO_MSEC_2_TICKS (100));
    }
    return DEF_ERR_SYS;
}

static ACE_err_t
start_run (void *nix)
{
    /* Kernel logging on */
    ACE_puts(ACE_ESC_CLEAR_SCREEN);
    USO_log_puts (USO_LL_INFO, SAM_BOARD" : "ACE_MOST_VERSION_STRING"\n");
    USO_log_puts (USO_LL_INFO, "Start: Kernel log on ttyS0.\n");

    unsigned long ticks_count = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS (100000L));
    USO_log_printf (USO_LL_INFO, "Loop calib 100ms: %lu.\n", USO_TICKS_2_MSEC(DEV_get_ticks_diff (ticks_count)) );

    USO_log_puts (USO_LL_INFO, "Timers start.\n");
    DEV_timers_start (TIMERS_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Run LED start.\n");
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ACE_ARRAYSIZE (run_led_stack), USO_SYSTEM, USO_FIFO, "run");
    USO_start (&run_led_thread);
    USO_yield();

    CLI_setup (CLI_config.hostname);
    tty1 = MFS_resolve("/sys/cli/tty1");
    if (tty1 == NULL)
    {
        ACE_PANIC ("Open tty1 fail");
    }
    CLI_switch_set(1,tty1);

    USO_log_puts (USO_LL_INFO, "Cli1 start on F2.\n");
    CLI_interpreter_start ("F2", tty1, CLI_STACK_SIZE, CLI_RUN_STACK_SIZE);

    USO_log_puts (USO_LL_INFO, "Cli0 start on F1.\n");
    CLI_interpreter_start ("F1", NULL, CLI_STACK_SIZE, CLI_RUN_STACK_SIZE);

    MFS_descriptor_t *dir = MFS_resolve("/bsp");
    SAM_bsp_commands_install (dir);
    MFS_close_desc(dir);
    
    DEV_digout_set (&SAM_green_led);
    SAM_main ();
    return ACE_OK;
}

extern void
SAM_start (ACE_FILE * stdio)
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
