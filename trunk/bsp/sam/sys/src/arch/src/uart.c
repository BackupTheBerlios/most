/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <dev/serial.h>
#include <dev/serial_settings.h>
#include <dev/arch/at91sam7x/uart.h>
#include <dev/arch/at91sam7x/AT91SAM7X256.h>

#include <arch/uart.h>

static DEV_serial_t ser0;
static struct DEV_serial_settings ser0_settings;

static DEV_serial_t ser1;
static struct DEV_serial_settings ser1_settings;

static void
open_0 (const struct DEV_serial_settings *settings)
{
    DEV_at91_uart_open (0, settings);
}

static void
open_1 (const struct DEV_serial_settings *settings)
{
    DEV_at91_uart_open (1, settings);
}

static void
close_0 (void)
{
    DEV_at91_uart_close (0);
}

static void
close_1 (void)
{
    DEV_at91_uart_close (1);
}

static void
start_0 (void)
{
    DEV_at91_uart_start (0, &ser0);
}

static void
start_1 (void)
{
    DEV_at91_uart_start (1, &ser1);
}

extern void
SAM_uart_init_0 (void)
{
    DEV_at91_uart0_init ();
    DEV_serial_settings_init (&ser0_settings);
    ser0_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser0, &ser0_settings, open_0, close_0, start_0, TRUE, "ser0");
}

extern void
SAM_uart_init_1 (void)
{
    DEV_at91_uart1_init ();
    DEV_serial_settings_init (&ser1_settings);
    ser1_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser1, &ser1_settings, open_1, close_1, start_1, TRUE, "ser1");
}

extern void
SAM_uart_interrupt_0 (void)
{
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_US0);
    DEV_at91_uart_interrupt (0, &ser0);
}

extern void
SAM_uart_interrupt_1 (void)
{
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_US1);
    DEV_at91_uart_interrupt (1, &ser1);
}
