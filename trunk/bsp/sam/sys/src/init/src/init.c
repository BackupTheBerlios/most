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

#include <ace/arch/cpu.h>
#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/scheduler.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/arch/at91/debug.h>
#include <dev/arch/at91/flashd.h>
#include <dev/arch/at91/rst.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <dev/mmc.h>
#include <cli/tty.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>

#include "arch/cpu.h"
#include "arch/reset.h"
#include "arch/pins.h"
#include "arch/exceptions.h"
#include "arch/uart.h"
#include "arch/eth.h"
#include "arch/ticks.h"
#include "arch/rtc.h"
#include "arch/digio.h"
#include "arch/spi.h"
#include "arch/adc.h"
#include "arch/pwm.h"
#include "init/main.h"
#include "init/init.h"
#include "init/start.h"
#include "init/config.h"
#include "init/events.h"

#define SAM_LOOPS_PER_MSEC 2250L;

#define RED_PAUSE 500000L       /* 0,5s = 500 000 us */
#define RED_PULSE 200L          /* 200 ms = 5 p/s */

#define GREEN_PULSE 200L         /* 200 ms = 5 p/s */

static ACE_FILE *ttyS0 = NULL;
static ACE_FILE *ser1 = NULL;

static CLI_tty_t tty_ser0;

extern char data_start, data_end, text_end;     /* Defined in *.ld! */
extern char bss_start, bss_end; /* Defined in *.ld! */
extern char stack_start;        /* stack_end defined in *.ld!, SAM_TOTAL_STACK_SIZE must match size of stack in *.ld */
extern char heap_start, heap_end;       /* Defined in *.ld! */

static USO_heap_t heap;

static void
blink_red (int c)
{
    while (1)
    {
        DEV_blink_nb (&SAM_red_led, c, RED_PULSE);
        DEV_cpudelay (RED_PAUSE);
    }
}

static void
blink_green (void)
{
    DEV_blink_nb (&SAM_green_led, 1, GREEN_PULSE);
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
    DEV_at91_FLASHD_initialize (SAM_MCK);
    CLI_tty_init (&tty_ser0,
                  MFS_SYSFS_DIR_SERIAL, "ser0", CLI_TTY_MODE_NL2CR, CLI_TTY_MODE_NL2CR, "ttyS0");

    SAM_uart_init_0 ();
    SAM_uart_init_1 ();
    SAM_rtc_init ();
    SAM_ticks_init ();
    SAM_sys_interrupt_init ();

    blink_green ();

    /* Initialize kernel logging */

    ttyS0 = MFS_get_stream (MFS_open (MFS_sysfs_get_dir (MFS_SYSFS_DIR_CLI), "ttyS0"));
    if (ttyS0 == NULL)
    {
        ACE_PANIC ("Open ttyS0 fail");
    }

    ser1 = MFS_get_stream (MFS_open (MFS_sysfs_get_dir (MFS_SYSFS_DIR_SERIAL), "ser1"));
    if (ser1 == NULL)
    {
        ACE_PANIC ("Open ser1 fail");
    }

    USO_log_init (ttyS0, USO_LL_INFO);
    USO_enable ();
    blink_green ();
    USO_log_puts (USO_LL_INFO, "\nKernel log on ttyS0.\n");

    /* Kernel logging on */

    unsigned long loop_count = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS (100000));
    USO_log_printf (USO_LL_INFO, "Loop calib 100ms: %lu.\n", DEV_get_ticks_diff (loop_count));

    if (SAM_spi_init () != 0)
    {
        DEV_digout_set (&SAM_red_led);
        USO_log_puts (USO_LL_ERROR, "SPI init failed.\n");
        SAM_config_init ();
    }
    else
    {
        if (DEV_mmc_spi_init ((DEV_spi_dev_t *) & SAM_mmc) < ACE_ERR_OK)
        {
            DEV_digout_set (&SAM_red_led);
            USO_log_puts (USO_LL_ERROR, "MMC spi init failed.\n");
            SAM_config_init ();
        }
        else if (DEV_mmc_init () < ACE_ERR_OK)
        {
            DEV_digout_set (&SAM_red_led);
            USO_log_puts (USO_LL_ERROR, "MMC card not found.\n");
            SAM_config_init ();
        }
        else
        {
            DEV_mmc_install ();
            SAM_config_read ();
        }
    }
    SAM_eth_init ();            /* must be called after SAM_config_read() */
    SAM_emac_interrupt_init ();
    SAM_adc_init ();
    SAM_pwm_init ();
    SAM_events_init ();

    /* ttyS0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use ser0 as stdio as long they don't change it.
     */
    SAM_start (ttyS0);
    USO_log_puts (USO_LL_INFO, "Idle.\n");
}

extern void
SAM_init (void)
{
    /* C setup: initialized variables */
    memcpy (&data_start, &text_end, &data_end - &data_start);

    /* C setup: not initialized variables */
    memset (&bss_start, 0, &bss_end - &bss_start);

    if (SAM_WDT_DISABLE)
    {
        SAM_wdt_disable ();
    }

    SAM_pins_init ();

    DEV_at91_RST_set_ext_reset_length (AT91C_BASE_RSTC, 0x8);
    DEV_at91_RST_set_user_reset_enable (AT91C_BASE_RSTC, 1);

    /* Low level debug (polling) initialization */
    DEV_at91_DBGU_configure ();
    DEV_at91_DBGU_print_ascii ("AT91SAM7X chip ID : ");
    DEV_at91_DBGU_print_hex8 (AT91C_BASE_DBGU->DBGU_CIDR);
    DEV_at91_DBGU_print_ascii ("\n");

    /* Use green, red LED for debugging */
    DEV_loops_per_msec = SAM_LOOPS_PER_MSEC;
    SAM_digio_init ();
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
    SAM_digio_install ();
    blink_green ();

    /* Go multithreading */
    USO_transform (init, (USO_stack_t *) & stack_start, SAM_IDLE_STACK_SIZE / sizeof (USO_stack_t));
}
