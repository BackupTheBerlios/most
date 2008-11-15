/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef DEV_AT91_UART_H
#define DEV_AT91_UART_H

#include "dev/serial.h"
#include "dev/serial_settings.h"

/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup at91
 *
 * @{
 */

/** @defgroup uart uart.h
 *
 * AT91 UART.
 *
 * @{
 */

/*-------------- Interface -----------------------------------------------*/

/**
 * Low level function to open serial port.
 *
 * @param port : 0 or 1
 * @param settings : initialized serial settings struct.
 */
extern void DEV_at91_uart_open (
	const struct DEV_serial_settings *settings
/*	void (*f) (void) */
	);

/**
 * Low level function to close serial port.
 *
 * @param port : 0 or 1
 */
extern void DEV_at91_uart_close (void);

/**
 * Low level function to start serial transmission.
 *
 * @param port : 0 or 1
 * @param serial : Serial devive
 */
extern void DEV_at91_uart_start (DEV_serial_t * serial);

/**
 * Low level function transmit end interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void DEV_at91_uart_interrupt (DEV_serial_t * serial);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

/** @}
 */

#endif
