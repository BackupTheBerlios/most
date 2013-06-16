/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef AT91SAM7A2_UART_H
#define AT91SAM7A2_UART_H

#include <dev/serial.h>
#include <dev/serial_settings.h>

/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup AT91SAM7A2
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

extern void AT91SAM7A2_uart_init (int port);

/**
 * Low level function to open serial port.
 *
 * @param port : 0 or 1
 * @param settings : initialized serial settings struct.
 */
extern void AT91SAM7A2_uart_open (int port, const struct DEV_serial_settings *settings);

/**
 * Low level function to close serial port.
 *
 * @param port : 0 or 1
 */
extern void AT91SAM7A2_uart_close (int port);

/**
 * Low level function to start serial transmission.
 *
 * @param port : 0 or 1
 * @param serial : Serial devive
 */
extern void AT91SAM7A2_uart_start (int port, DEV_serial_t * serial);

/**
 * Low level function transmit end interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void AT91SAM7A2_uart_interrupt (int port, DEV_serial_t * serial);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

/** @}
 */

#endif
