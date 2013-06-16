/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <dev/serial.h>
#include <dev/serial_settings.h>
#include <dev/arch/at91sam7a2/uart.h>
#include <dev/arch/at91sam7a2/csp.h>

#include <arch/uart.h>

static DEV_serial_t ser0;
static struct DEV_serial_settings ser0_settings;

static DEV_serial_t ser1;
static struct DEV_serial_settings ser1_settings;

static void
open_0 (const struct DEV_serial_settings *settings)
{
	AT91SAM7A2_uart_open (0, settings);
}

static void
open_1 (const struct DEV_serial_settings *settings)
{
	AT91SAM7A2_uart_open (1, settings);
}

static void
close_0 (void)
{
	AT91SAM7A2_uart_close (0);
}

static void
close_1 (void)
{
	AT91SAM7A2_uart_close (1);
}

static void
start_0 (void)
{
	AT91SAM7A2_uart_start (0, &ser0);
}

static void
start_1 (void)
{
	AT91SAM7A2_uart_start (1, &ser1);
}

extern void
LA2_uart_init_0 (void)
{
	AT91SAM7A2_uart_init (0);
    DEV_serial_settings_init (&ser0_settings);
    ser0_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser0, &ser0_settings, open_0, close_0, start_0, TRUE, "ser0");

    /* Configure USART controller interrupt mode in GIC module */
    CSP_GICConfigInterrupt(CSP_GET_INTERRUPT_NUMBER(USART0), HIGH_LEVEL_SENSITIVE | PRIOR_4, (U32_T)LA2_uart_interrupt_0);
}

extern void
LA2_uart_init_1 (void)
{
	AT91SAM7A2_uart_init (1);
    DEV_serial_settings_init (&ser1_settings);
    ser1_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser1, &ser1_settings, open_1, close_1, start_1, TRUE, "ser1");

    /* Configure USART controller interrupt mode in GIC module */
    CSP_GICConfigInterrupt(CSP_GET_INTERRUPT_NUMBER(USART1), HIGH_LEVEL_SENSITIVE | PRIOR_4, (U32_T)LA2_uart_interrupt_1);
}

extern void
LA2_uart_interrupt_0 (void)
{
    CSP_GIC_SET_ICCR(GIC, IBIT_USART0);
    AT91SAM7A2_uart_interrupt (0, &ser0);
}

extern void
LA2_uart_interrupt_1 (void)
{
    CSP_GIC_SET_ICCR(GIC, IBIT_USART1);
    AT91SAM7A2_uart_interrupt (1, &ser1);
}
