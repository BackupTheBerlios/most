/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_SERIAL_ERROR_H
#define DEV_SERIAL_ERROR_H

/** @addtogroup dev
 * @{
 */


/** @defgroup serial_error serial_error.h
 *
 * Serial Errors.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** Low level receive errors */
enum DEV_serial_rx_err
{

    /** Overrun Error*/
    DEV_SER_RX_OVERRUN,

    /** Framing Error*/
    DEV_SER_RX_FRAMING,

    /** Parity Error */
    DEV_SER_RX_PARITY
};

/**
 * Serial error struct.
 *
 * Public
 */
struct DEV_serial_error
{

    /** LL overrun error */
    unsigned long rx_overrun;

    /** LL framing error */
    unsigned long rx_framing;

    /** LL parity error */
    unsigned long rx_parity;

    /** Receive buffer overrun */
    unsigned long rx_buf_overrun;

    /** Transmit buffer overrun */
    unsigned long tx_buf_overrun;
};

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize serial error struct.
 *
 * Set all errors to 0:
 *
 * @param error : Pointer to serial error struct.
 */
extern void DEV_serial_error_init (struct DEV_serial_error *error);

/**
 * Print serial error struct to stdout.
 *
 * @param error : Pointer to serial error struct.
 */
extern void DEV_serial_error_print (struct DEV_serial_error *error);

/**
 * Count serial low level error.
 *
 * @param error : Pointer to serial error struct.
 * @param err_code : Low level error code.
 */
extern void DEV_serial_error_rx (struct DEV_serial_error *error, enum DEV_serial_rx_err err_code);

/*------------------------------------------------------------------------*/

/** @}
 */


/** @}
 */

#endif
