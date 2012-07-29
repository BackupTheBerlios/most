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

extern void DEV_at91_uart0_init (void);

extern void DEV_at91_uart1_init (void);

/**
 * Low level function to open serial port.
 *
 * @param port : 0 or 1
 * @param settings : initialized serial settings struct.
 */
extern void DEV_at91_uart_open (int port, const struct DEV_serial_settings *settings);

/**
 * Low level function to close serial port.
 *
 * @param port : 0 or 1
 */
extern void DEV_at91_uart_close (int port);

/**
 * Low level function to start serial transmission.
 *
 * @param port : 0 or 1
 * @param serial : Serial devive
 */
extern void DEV_at91_uart_start (int port, DEV_serial_t * serial);

/**
 * Low level function transmit end interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void DEV_at91_uart_interrupt (int port, DEV_serial_t * serial);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

/** @}
 */

#endif
