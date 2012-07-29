/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/arch/cpu.h>
#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/scheduler.h>
#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <dev/chips/flash_29F040.h>
#include <mfs/sysfs.h>

#include "arch/cpu.h"
#include "arch/spi.h"
#include "arch/sci.h"
#include "arch/digio.h"
#include "arch/ticks.h"
#include "arch/eth.h"
#include "init/init.h"
#include "init/start.h"
#include "init/config.h"
#include "mfs/descriptor.h"
#include "mfs/stream.h"
#include "mfs/sysfs.h"
#include "arch/digio.h"

#define MDC_LOOPS_PER_MSEC 175L

#define RED_PAUSE 250000L       /* 0,25s = 250 000 us */
#define RED_PULSE 200L          /* 200 ms = 5 p/s */

#define GREEN_PULSE 200L         /* 200 ms = 5 p/s */

#define IDLE_STACK_SIZE       0x400

extern char data_start, data_end, code_end;     /* Defined in *.ld! */
extern char bss_start, bss_end; /* Defined in *.ld! */
extern char heap_start, heap_end;       /* Defined in *.ld! */
extern char stack_start;        /* stack_end defined in *.ld!, stack size must match IDLE_STACK_SIZE */

static ACE_FILE *ser0 = NULL;
static ACE_FILE *ser1 = NULL;

static USO_heap_t heap;

static void
blink_red (int c)
{
    int i = c;
    while (1)
    {
        while (i--)
        {
            MDC_watchdog_trigger();
            DEV_blink_nb (&MDC_red_led, 1, RED_PULSE);
        }
        MDC_watchdog_trigger();
        DEV_cpudelay (RED_PAUSE);
        MDC_watchdog_trigger();
        DEV_cpudelay (RED_PAUSE);
        MDC_watchdog_trigger();
        i = c;
    }
}

static void
blink_green (void)
{
    MDC_watchdog_trigger();
    DEV_blink_nb (&MDC_green_led, 1, GREEN_PULSE);
    MDC_watchdog_trigger();
}

static void
panic_handler (char *msg, char *file, int line)
{
    USO_disable ();
    blink_red (1);
}

static void
abort_handler (char *msg, char *file, int line)
{
    USO_log_printf (USO_LL_PANIC, "\n PANIC: %s - %s - [%s, %d] abort!\n",
                 msg, USO_thread_name (), file, line);
    USO_sleep (USO_MSEC_2_TICKS (1000));
    USO_disable ();
    blink_red (2);
}

static void
init (void)
{
    /* System initialization without kernel logging */

    ACE_stdio_init ();
    USO_sleep_init ();
    DEV_timers_init ();

    FLASH_29F040_set_wtd_trigger (MDC_watchdog_trigger);
    MDC_sci_init_0 ();
    MDC_sci_init_1 ();
    MDC_ticks_init ();

    blink_green ();

    /* Initialize kernel logging */

    ser0 = MFS_get_stream (MFS_open (MFS_sysfs_get_dir (MFS_SYSFS_DIR_SERIAL), "0"));
    if (ser0 == NULL)
    {
        ACE_PANIC ("Open ser0 fail");
    }

    ser1 = MFS_get_stream (MFS_open (MFS_sysfs_get_dir (MFS_SYSFS_DIR_SERIAL), "1"));
    if (ser1 == NULL)
    {
        ACE_PANIC ("Open ser1 fail");
    }

    USO_log_init (ser0, USO_LL_INFO);
    USO_enable ();
    blink_green ();
    USO_log_puts (USO_LL_INFO, "\nKernel log on ser0.\n");

    /* Kernel logging on*/

    unsigned long loop_count = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS (100000L));
    USO_log_printf (USO_LL_INFO, "Loop calib 100ms: %lu.\n", DEV_get_ticks_diff (loop_count));

    if (MDC_spi_init () != 0)
    {
        DEV_digout_set (&MDC_red_led);
        USO_log_puts (USO_LL_ERROR, "SPI init failed.\n");
        MDC_config_init ();
    }
    else
    {
        if (MDC_ee_init() < ACE_ERR_OK)
        {
            DEV_digout_set (&MDC_red_led);
            USO_log_puts (USO_LL_ERROR, "EE init failed.\n");
            MDC_config_init ();
        }
        else
        {
            MDC_config_read ();
        }
    }

    MDC_eth_init ();            /* must be called after ee_config_read() */



    /* ser0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use ser0 as stdio as long they don't change it.
     */
    MDC_start (ser0);
    USO_log_puts (USO_LL_INFO, "Idle.\n");
}

extern void
MDC_init (void)
{
    MDC_cpu_init ();
    MDC_watchdog_trigger();

    /* C setup: initialized variables */
    memcpy (&data_start, &code_end, &data_end - &data_start);
    /* C setup: not initialized variables */
    memset (&bss_start, 0, &bss_end - &bss_start);

    /* Use green, red LED for debugging */
    DEV_loops_per_msec = MDC_LOOPS_PER_MSEC;
    MDC_digio_init ();
    /* blink_red (10); */
    blink_green ();

    /* Abort handler initialization */
    ACE_stdlib_init (&heap, abort_handler, panic_handler);

    /* Heap initialization */
    if (USO_heap_init (&heap, &heap_start, &heap_end) == FALSE)
    {
        ACE_PANIC ("Heap init fail");
    }
    blink_green ();
    if (MFS_sysfs_init () == FALSE)
    {
        ACE_PANIC ("Sysfs init fail");
    }
    USO_heap_install (&heap, "heap0");
    MDC_digio_install ();
    blink_green ();

    /* Go multithreading */
    USO_transform (init, (USO_stack_t *) & stack_start, IDLE_STACK_SIZE / sizeof (USO_stack_t));
}
