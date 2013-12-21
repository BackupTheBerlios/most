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
#include <uso/buf_pool.h>
#include <uso/scheduler.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <cli/tty.h>
#include <cli/switch.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>
#include <dev/arch/at91sam7a2/csp.h>
#include <cli/config.h>

#include <arch/cpu.h>
#include <arch/reset.h>
#include <arch/exceptions.h>
#include <arch/pins.h>
#include <arch/digio.h>
#include <arch/ticks.h>
#include <arch/rtc.h>
#include <arch/uart.h>
#include <arch/pwm.h>
#include <arch/adc.h>
#include <arch/spi.h>
#include <arch/mmc.h>
#include <init/init.h>
#include <init/events.h>
#include <init/start.h>
#include <init/config.h>


extern char data_start, data_end, ramc_start, ramc_end, code_end;     /* Defined in *.ld! */
extern char bss_start, bss_end; /* Defined in *.ld! */
extern char stack_start;        /* stack_end defined in *.ld!, SAM_TOTAL_STACK_SIZE must match size of stack in *.ld */
extern char heap_start, heap_end;       /* Defined in *.ld! */


#define LA2_LOOPS_PER_MSEC 420L;
#define RED_PAUSE (1000000L)       /* 1sec */
#define RED_PULSE (200L)          /* 5 p/s */
#define GREEN_PULSE (200L)         /* 5 p/s */

/* !!!! you must have more or same amount of ios buffers as CLIs otherwise the system deadlocks !!!!!
 * want to get rid of ios bufs (sprintf like sscanf)
 */
#define IOS_BUF_COUNT 8
#define IOS_BUF_SIZE 0x100

static USO_buf_pool_t ios_buf_pool;
static char ios_bufs[IOS_BUF_COUNT][IOS_BUF_SIZE];

static MFS_descriptor_t *ser0 = NULL;
static MFS_descriptor_t *ser1 = NULL;
static MFS_descriptor_t *tty0 = NULL;

static CLI_tty_t tty_0;
static CLI_tty_t tty_1;
static CLI_tty_t tty_2;
static CLI_tty_t tty_3;

static USO_heap_t heap;

static void
blink_red (int i)
{
    DEV_blink_nb (&LA2_red_led, i, RED_PULSE);
}

static void
blink_green (int i)
{
    DEV_blink_nb (&LA2_green_led, i, GREEN_PULSE);
}

static void
panic_handler (char *msg, char *file, int line)
{
    USO_disable ();
    while (1){
        blink_red (20);
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
    }
}

static void
abort_handler (char *msg, char *file, int line)
{
    USO_log_printf (USO_LL_PANIC, "\n ABORT: %s - %s - [%s, %d] !\n",
                 msg, USO_thread_name (), file, line);
    USO_sleep (USO_MSEC_2_TICKS (200));
    USO_disable ();
    while (1){
        blink_red (10);
        DEV_cpudelay (DEV_USEC_2_LOOPS(RED_PAUSE));
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
    LA2_uart_init_0 ();
    LA2_uart_init_1 ();
    LA2_ticks_init ();
    LA2_rtc_init ();
    LA2_events_init ();
    LA2_pwm_init ();
    LA2_adc_install ();

    blink_green (1);

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

    CLI_tty_init (&tty_2,
                  ser0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty2",
                  FALSE);

    CLI_tty_init (&tty_3,
                  ser0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty3",
                  FALSE);

    CLI_switch_init();
    CLI_switch_set(0,tty0);
    CLI_tty_select(tty0);

    ser1 = MFS_resolve("/sys/dev/serial/ser1");
    if (ser1 == NULL)
    {
        ACE_PANIC ("Open ser1 fail");
    }

    /* Interrupts must be enabled before starting the first thread! */
    USO_enable ();

    CLI_config_init ();
    LA2_config_install (); /* config install must be done before read */
    if (LA2_spi_init () != 0)
    {
        ACE_PANIC ("SPI init fail");
    }
    else
    {
        if (LA2_mmc_create () == ACE_OK)
        {
            CLI_config_read ();
        } else {
            blink_red (2);
        }
    }

    /* The idle thread is not allowed to block
     * so it is not allowed to do any printf or log output!!!!
     */
    USO_log_init (tty0, USO_LL_INFO);
    blink_green (1);

    blink_green (1);

    /* ttyS0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use ttyS0 as stdio as long they don't change it.
     */
    LA2_start (tty0);
    /* idle */
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
    LA2_digio_install ();
    blink_green (1);

    /* Go multithreading */
    USO_transform (idle_run, (USO_stack_t *) &stack_start, LA2_IDLE_STACK_SIZE / sizeof (USO_stack_t));
}
