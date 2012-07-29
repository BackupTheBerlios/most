/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef DEV_H8_SCI_H
#define DEV_H8_SCI_H

#include "dev/serial.h"
#include "dev/serial_settings.h"

/** @addtogroup dev
 *
 * @{
 */


/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup h8
 *
 * @{
 */

/** @defgroup sci sci.h
 *
 * H8300 Serial Communication Interface.
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
extern void DEV_h8_sci_open (int port, const struct DEV_serial_settings *settings);

/**
 * Low level function to close serial port.
 *
 * @param port : 0 or 1
 */
extern void DEV_h8_sci_close (int port);

/**
 * Low level function to start serial transmission.
 *
 * @param port : 0 or 1
 * @param serial : Serial devive
 */
extern void DEV_h8_sci_start (int port, DEV_serial_t * serial);

/**
 * Low level function transmit end interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void DEV_h8_sci_txe (int port, DEV_serial_t * serial);

/**
 * Low level function transmit interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void DEV_h8_sci_txi (int port, DEV_serial_t * serial);

/**
 * Low level function receive interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void DEV_h8_sci_rxi (int port, DEV_serial_t * serial);

/**
 * Low level function receive error interrupt.
 *
 * @param port : 0 or 1
 * @param serial : Serial device
 */
extern void DEV_h8_sci_eri (int port, DEV_serial_t * serial);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

/** @}
 */

/** @}
 */


#endif
