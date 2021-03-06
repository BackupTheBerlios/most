/*
 * This file is part of MOST.
 *
 * MOST is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MOST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MOST.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/scheduler.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/arch/at91sam7x/debug.h>
#include <dev/arch/at91sam7x/flashd.h>
#include <dev/arch/at91sam7x/rst.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>
#include <cli/tty.h>
#include <cli/switch.h>
#include <cli/config.h>

#include <arch/cpu.h>
#include <arch/reset.h>
#include <arch/pins.h>
#include <arch/exceptions.h>
#include <arch/uart.h>
#include <arch/ticks.h>
#include <arch/rtc.h>
#include <arch/digio.h>
#include <arch/spi.h>
#include <arch/adc.h>
#include <arch/pwm.h>
#include <arch/eth.h>
#include <arch/mmc.h>
#include <init/main.h>
#include <init/init.h>
#include <init/start.h>
#include <init/config.h>
#include <init/events.h>

extern char data_start, data_end, code_end;     /* Defined in *.ld! */
extern char bss_start, bss_end; /* Defined in *.ld! */
extern char stack_start;        /* stack_end defined in *.ld!, SAM_TOTAL_STACK_SIZE must match size of stack in *.ld */
extern char heap_start, heap_end;       /* Defined in *.ld! */


#define SAM_LOOPS_PER_MSEC 2000L;

#define RED_PAUSE (1000000L)       /* 1sec */
#define RED_PULSE (200L)          /* 5 p/s */

#define GREEN_PULSE (200L)         /* 5 p/s */

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
    DEV_blink_nb (&SAM_red_led, c, RED_PULSE);
}

static void
blink_green (int c)
{
    DEV_blink_nb (&SAM_green_led, c, GREEN_PULSE);
}

static void
panic_handler (char *msg, char *file, int line)
{
    USO_disable ();
    DEV_at91_DBGU_print_ascii ("\nPANIC: ");
    DEV_at91_DBGU_print_ascii (msg);
    DEV_at91_DBGU_print_ascii (" - ");
    DEV_at91_DBGU_print_ascii (USO_thread_name ());
    DEV_at91_DBGU_print_ascii (" - [");
    DEV_at91_DBGU_print_ascii (file);
    DEV_at91_DBGU_print_ascii ("] !\n");
    for (;;){
        blink_red (20);
        DEV_cpudelay (DEV_USEC_2_LOOPS (RED_PAUSE));
    }
}

static void
abort_handler (char *msg, char *file, int line)
{
    USO_log_printf (USO_LL_PANIC, "\n ABORT: %s - %s - [%s, %d] !\n",
                 msg, USO_thread_name (), file, line);
    for (;;){
        blink_red (10);
        USO_sleep (USO_MSEC_2_TICKS (RED_PAUSE/1000));
    }
}

static void
idle_run (void)
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
    DEV_at91_FLASHD_initialize (SAM_MCK);

    SAM_uart_init_0 ();
    SAM_uart_init_1 ();
    SAM_rtc_init ();
    SAM_ticks_init ();
    SAM_sys_interrupt_init ();
    SAM_adc_init ();
    SAM_pwm_init ();
    SAM_events_init ();

    blink_green (1);

    ser0 = MFS_resolve("/sys/dev/serial/ser0");
    if (ser0 == NULL)
    {
        ACE_PANIC ("Open ser0 fail");
    }

    /* Initialize kernel logging */
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

    CLI_switch_init();
    CLI_switch_set(0,tty0);
    CLI_tty_select(tty0);

    ser1 = MFS_resolve("/sys/dev/serial/ser1");
    if (ser1 == NULL)
    {
        ACE_PANIC ("Open ser1 fail");
    }

    /* The idle thread is not allowed to block
     * so it is not allowed to do any printf or log output!!!!
     */
    USO_log_init (tty0, USO_LL_INFO);
    blink_green (1);

    /* Interrupts must be enabled before starting the first thread! */
    USO_enable ();
    blink_green (1);

    CLI_config_init ();
    SAM_config_install ();
    if (SAM_spi_init () != 0)
    {
        ACE_PANIC ("Spi init fail");
    }
    else
    {
        if (SAM_mmc_install () == ACE_OK)
        {
            CLI_config_read ();
        } else {
            blink_red(2);
        }
    }

    /* ttyS0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use ttyS0 as stdio as long they don't change it.
     */
    SAM_start (tty0);
    /* idle */
}

extern void
SAM_init (void)
{
    /* C setup: initialized variables */
    memcpy (&data_start, &code_end, &data_end - &data_start);

    /* C setup: not initialized variables */
    memset (&bss_start, 0, &bss_end - &bss_start);

    /* the wdt mode register is write-once! */
    if (SAM_WDT_DISABLE)
    {
        SAM_wdt_disable (); /* disable also writes the wdt mode register */
    } else {
        SAM_wdt_setup (SAM_WDV_10SEC);   /* initial wtv is 16 sec, but we may come from bootloader */
    }

    SAM_pins_init ();

    DEV_at91_RST_set_user_reset_enable (AT91C_BASE_RSTC, 1);

    /* Low level debug (polling) initialization */
    DEV_at91_DBGU_configure ();
    DEV_at91_DBGU_print_ascii ("AT91SAM7X chip ID : ");
    DEV_at91_DBGU_print_hex8 (AT91C_BASE_DBGU->DBGU_CIDR);
    DEV_at91_DBGU_print_ascii ("\n");

    /* Use green, red LED for debugging */
    DEV_loops_per_msec = SAM_LOOPS_PER_MSEC;
    SAM_digio_init ();
    blink_green (1);

    /* Abort handler initialization */
    ACE_stdlib_init (&heap, abort_handler, panic_handler);

    /* Heap initialization */
    if (USO_heap_init (&heap, &heap_start, &heap_end) == FALSE)
    {
        ACE_PANIC ("Heap init failed!");
    }
    /* SYS FS needs heap */
    if (MFS_sysfs_init () == FALSE)
    {
        ACE_PANIC ("Sysfs init failed!");
    }
    USO_heap_install (&heap, "heap0");
    SAM_digio_install ();
    blink_green (1);

    /* Go multithreading */
    USO_transform (idle_run, (USO_stack_t *) &stack_start, SAM_IDLE_STACK_SIZE / sizeof (USO_stack_t));
}
