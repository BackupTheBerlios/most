/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <dev/serial.h>
#include <dev/serial_settings.h>
#include <dev/arch/at91/uart.h>

#include "arch/uart.h"

static DEV_serial_t ser0;
static struct DEV_serial_settings ser0_settings;

extern void SAM_uart_interrupt_0 (void)
{
    DEV_at91_uart_interrupt (& ser0);
}

static void
open_0 (const struct DEV_serial_settings *settings)
{
    DEV_at91_uart_open (settings);
}

static void
close_0 (void)
{
    DEV_at91_uart_close ();
}

static void
start_0 (void)
{
    DEV_at91_uart_start (& ser0);
}

void
SAM_uart_init_0 (void)
{
    DEV_serial_settings_init (&ser0_settings);
    ser0_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser0, &ser0_settings, open_0, close_0, start_0, TRUE, "0");
}
