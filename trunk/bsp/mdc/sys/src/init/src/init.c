/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

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
#include <dev/chips/flash_29F040.h>
#include <dev/cpu.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>
#include <mfs/sysfs.h>
#include <cli/tty.h>
#include <cli/switch.h>

#include <arch/cpu.h>
#include <arch/pins.h>
#include <arch/sci.h>
#include <arch/digio.h>
#include <arch/ticks.h>
#include <arch/interrupts.h>
#include <arch/spi.h>
#include <arch/ee.h>
#include <init/init.h>
#include <init/start.h>

extern char data_start, data_end, code_end;     /* Defined in *.ld! */
extern char bss_start, bss_end; /* Defined in *.ld! */
extern char heap_start, heap_end;       /* Defined in *.ld! */
extern char stack_start;        /* stack_end defined in *.ld!, stack size must match IDLE_STACK_SIZE */

#define MDC_LOOPS_PER_MSEC 175L
#define RED_PAUSE (500000L)       /* 0.5sec */
#define RED_PULSE (200L)          /* 5 p/s */
#define GREEN_PULSE (200L)         /* 5 p/s */
#define IDLE_STACK_SIZE       0x400
/* !!!! you must have more or same amount of ios buffers as CLIs otherwise the system deadlocks !!!!!
 */
#define IOS_BUF_COUNT 4
#define IOS_BUF_SIZE 0x100

static USO_buf_pool_t ios_buf_pool;
static char ios_bufs[IOS_BUF_COUNT][IOS_BUF_SIZE];

static USO_heap_t heap;

static MFS_descriptor_t *ser0 = NULL;
static MFS_descriptor_t *ser1 = NULL;
static MFS_descriptor_t *tty0 = NULL;

static CLI_tty_t tty_0;
static CLI_tty_t tty_1;


static void
blink_red (int c)
{
    MDC_watchdog_trigger();
    DEV_blink_nb (&MDC_red_led, c, RED_PULSE);
    MDC_watchdog_trigger();
}

static void
blink_green (int c)
{
    MDC_watchdog_trigger();
    DEV_blink_nb (&MDC_green_led, c, GREEN_PULSE);
    MDC_watchdog_trigger();
}

static void
panic_handler (char *msg, char *file, int line)
{
    USO_disable ();
    while (1)
    {
        blink_red (20);
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
        MDC_watchdog_trigger();
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
    }
}

static void
abort_handler (char *msg, char *file, int line)
{
    USO_log_printf (USO_LL_PANIC, "\n PANIC: %s - %s - [%s, %d] abort!\n",
                 msg, USO_thread_name (), file, line);
    USO_sleep (USO_MSEC_2_TICKS (200));
    USO_disable ();
    while (1)
    {
        blink_red (10);
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
        MDC_watchdog_trigger();
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
    }
}

static void
idle (void)
{
    /* System initialization without kernel logging!
     * The scheduler is initialized and we are running on the idle
     * thread so now we are allowed to start threads!
     * The idle thread is not allowed to block!
     */

    USO_buf_pool_init (&ios_buf_pool, ios_bufs, IOS_BUF_COUNT, IOS_BUF_SIZE);
    ACE_stdio_init (&ios_buf_pool);
    USO_sleep_init ();
    DEV_timers_init ();
    FLASH_29F040_set_wtd_trigger (MDC_watchdog_trigger);
    MDC_sci_init_0 ();
    MDC_sci_init_1 ();
    MDC_ticks_init ();
    MDC_interrupts_init();

    blink_green (1);

    /* Initialize kernel logging */

    ser0 = MFS_resolve("/sys/dev/serial/ser0");
    if (ser0 == NULL)
    {
        ACE_PANIC ("Open ser0 fail");
    }

    CLI_tty_init (&tty_0,
                  ser0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty0",
                  TRUE);

    tty0 = MFS_resolve("/sys/cli/tty0");
    if (tty0 == NULL)
    {
        ACE_PANIC ("Open tty0 fail");
    }

    CLI_tty_init (&tty_1,
                  ser0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty1",
                  FALSE);

    ser1 = MFS_resolve("/sys/dev/serial/ser1");
    if (ser1 == NULL)
    {
        ACE_PANIC ("Open ser1 fail");
    }

    CLI_switch_init();
    CLI_switch_set(0,tty0);
    CLI_tty_select(tty0);

    if (MDC_spi_init () != 0)
    {
        ACE_PANIC ("SPI init fail");
    }
    if (MDC_ee_init() < ACE_OK)
    {
        ACE_PANIC ("EE init fail");
    }

    /* The idle thread is not allowed to block
     * so it is not allowed to do any printf or log output!!!!
     */
    USO_log_init (tty0, USO_LL_INFO);
    blink_green (1);

    /* Interrupts must be enabled before starting the first thread! */
    USO_enable ();
    blink_green (1);

    /* tty0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use tty0 as stdio as long they don't change it.
     */
    MDC_start (tty0);
    /* idle */
}

extern void
MDC_init (void)
{
    MDC_cpu_init ();
    /* C setup: initialized variables */
    memcpy (&data_start, &code_end, &data_end - &data_start);
    /* C setup: not initialized variables */
    memset (&bss_start, 0, &bss_end - &bss_start);

    MDC_pins_init (); /* trigger the watchdog very early, we can be run from bootloader */
    MDC_watchdog_trigger();
    /* Use green, red LED for debugging */
    DEV_loops_per_msec = MDC_LOOPS_PER_MSEC;
    MDC_digio_init ();
    blink_green (1);

    /* Abort handler initialization */
    ACE_stdlib_init (&heap, abort_handler, panic_handler);

    /* Heap initialization */
    if (USO_heap_init (&heap, &heap_start, &heap_end) == FALSE)
    {
        ACE_PANIC ("Heap init fail");
    }
    blink_green (1);
    if (MFS_sysfs_init () == FALSE)
    {
        ACE_PANIC ("Sysfs init fail");
    }
    USO_heap_install (&heap, "heap0");
    MDC_digio_install ();
    blink_green (1);

    /* Go multithreading */
    USO_transform (idle, (USO_stack_t *) & stack_start, IDLE_STACK_SIZE / sizeof (USO_stack_t));
}
