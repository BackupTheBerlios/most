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

#include <ace/cpu.h>
#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/buf_pool.h>
#include <uso/scheduler.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <cli/tty.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>
#include <dev/arch/at91sam7a2/csp.h>

#include <arch/cpu.h>
#include <arch/reset.h>
#include <arch/exceptions.h>
#include <arch/pins.h>
#include <arch/digio.h>
#include <arch/ticks.h>
#include <arch/rtc.h>
#include <arch/uart.h>
#include <arch/pwm.h>
#include <arch/spi.h>
#include <arch/mmc.h>
#include <arch/adc.h>
#include <init/init.h>
#include <init/config.h>
#include <init/events.h>
#include <init/start.h>

#define LA2_LOOPS_PER_MSEC 420L;

#define RED_PAUSE (1000000L)       /* 1sec */
#define RED_PULSE (200L)          /* 5 p/s */

#define GREEN_PULSE (200L)         /* 5 p/s */

extern char data_start, data_end, ramc_start, ramc_end, code_end;     /* Defined in *.ld! */
extern char bss_start, bss_end; /* Defined in *.ld! */
extern char stack_start;        /* stack_end defined in *.ld!, SAM_TOTAL_STACK_SIZE must match size of stack in *.ld */
extern char heap_start, heap_end;       /* Defined in *.ld! */

static MFS_descriptor_t *ser0 = NULL;
static MFS_descriptor_t *ser1 = NULL;
static MFS_descriptor_t *tty0 = NULL;

static CLI_tty_t tty_0;

static USO_heap_t heap;

static void
blink_red (int c)
{
    while (1)
    {
        DEV_blink_nb (&LA2_red_led, c, RED_PULSE);
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
    }
}

static void
blink_green (void)
{
    DEV_blink_nb (&LA2_green_led, 1, GREEN_PULSE);
}

static void
panic_handler (char *msg, char *file, int line)
{
    USO_disable ();
    blink_red (20);
}

static void
abort_handler (char *msg, char *file, int line)
{
    USO_log_printf (USO_LL_PANIC, "\n ABORT: %s - %s - [%s, %d] !\n",
                 msg, USO_thread_name (), file, line);
    USO_sleep (USO_MSEC_2_TICKS (200));
    USO_disable ();
    blink_red (10);
}

#define IOS_BUF_COUNT 3
#define IOS_BUF_SIZE 0x100
static USO_buf_pool_t ios_buf_pool;
static char ios_bufs[IOS_BUF_COUNT][IOS_BUF_SIZE];

static void
init (void)
{
	/* System initialization without kernel logging */

	USO_buf_pool_init (&ios_buf_pool, ios_bufs, IOS_BUF_COUNT, IOS_BUF_SIZE);
    ACE_stdio_init (&ios_buf_pool);
    USO_sleep_init ();
    DEV_timers_init ();
    LA2_uart_init_0 ();
    LA2_uart_init_1 ();
    LA2_ticks_init ();
    LA2_rtc_init ();

    blink_green ();

    ser0 = MFS_open (MFS_resolve(MFS_get_root(), "sys/dev/serial"), "ser0");
    if (ser0 == NULL)
    {
        ACE_PANIC ("Open ser0 fail");
    }

    CLI_tty_init (&tty_0,
                  ser0, ser0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty0");

    tty0 = MFS_open (MFS_resolve(MFS_get_root(), "sys/cli"), "tty0");
    if (tty0 == NULL)
    {
        ACE_PANIC ("Open tty0 fail");
    }

    ser1 = MFS_open (MFS_resolve(MFS_get_root(), "sys/dev/serial"), "ser1");
    if (ser1 == NULL)
    {
        ACE_PANIC ("Open ser1 fail");
    }

    USO_log_init (tty0, USO_LL_INFO);
    USO_enable ();
    blink_green ();
    USO_log_puts (USO_LL_INFO, "\nInit: Kernel log on tty0.\n");

    /* Kernel logging on */

    unsigned long ticks_count = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS (100000L));
    USO_log_printf (USO_LL_INFO, "Loop calib 100ms: %lu.\n", USO_TICKS_2_MSEC(DEV_get_ticks_diff (ticks_count)) );

    LA2_config_init ();

    if (LA2_spi_init () != 0)
    {
        DEV_digout_set (&LA2_red_led);
        USO_log_puts (USO_LL_ERROR, "SPI init failed.\n");
    }
    else
    {
        if (LA2_mmc_install () == ACE_OK)
        {
            LA2_config_read ();
        } else {
            DEV_digout_set (&LA2_red_led);
        }
    }

    LA2_events_init ();
    LA2_pwm_init ();
    LA2_adc_install ();
    
    /* ttyS0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use ttyS0 as stdio as long they don't change it.
     */
    LA2_start (tty0);

    USO_log_puts (USO_LL_INFO, "Idle.\n");
}

extern void
LA2_init (void)
{
    LA2_cpu_init ();

    /*Setup: vector table and C setup: initialized variables */
    memcpy (&data_start, &code_end, &data_end - &data_start);

    /* Setup: ram code */
    memcpy (&ramc_start, &code_end + (&data_end - &data_start), &ramc_end - &ramc_start);

    /* C setup: not initialized variables */
    memset (&bss_start, 0, &bss_end - &bss_start);

    /* Disable Watchdog */
    CSP_WD_T*    pWD;
    pWD = (CSP_WD_T*) WD_BASE_ADDRESS ;
    CSP_WD_SET_OMR(pWD, ((CSP_WD_GET_OMR(pWD) & (~WDEN)) | OKEY_ALLOW));

    LA2_pins_init ();

    /* Use green, red LED for debugging */
    DEV_loops_per_msec = LA2_LOOPS_PER_MSEC;

    LA2_digio_init ();
    blink_green ();

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
    LA2_digio_install ();
    blink_green ();

    /* Go multithreading */
    USO_transform (init, (USO_stack_t *) &stack_start, LA2_IDLE_STACK_SIZE / sizeof (USO_stack_t));
}
