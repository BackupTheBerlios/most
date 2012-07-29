/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_SERIAL_SETTINGS_H
#define DEV_SERIAL_SETTINGS_H

#include <uso/sleep.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup serial_settings serial_settings.h
 *
 * Serial Settings.
 * @{
 */

/*------------- Representation ------------------------------------------*/

#define DEV_SER_TIMEOUT_MSEC 50L

/** Size of receive buffer. */
#define DEV_SERIAL_B_RX_BUFFER_SIZE 0x100
/** Size of receive buffer for nonblocking mode. */
#define DEV_SERIAL_NB_RX_BUFFER_SIZE 0x400

/** Size of transmit buffer. */
#define DEV_SERIAL_B_TX_BUFFER_SIZE 0x100
/** Size of transmit buffer for nonblocking mode. */
#define DEV_SERIAL_NB_TX_BUFFER_SIZE 0x400

/** Receive timeout. */
#define DEV_SER_TIMEOUT USO_MSEC_2_TICKS(DEV_SER_TIMEOUT_MSEC)

/** Baud rate. */
enum DEV_serial_baud
{

    /** 1200. */
    DEV_SER_BAUD_1200,

    /** 2400. */
    DEV_SER_BAUD_2400,

    /** 4800. */
    DEV_SER_BAUD_4800,

    /** 9600. */
    DEV_SER_BAUD_9600,

    /** 19200. */
    DEV_SER_BAUD_19200,

    /** 38400. */
    DEV_SER_BAUD_38400
};

/** Data bits. */
enum DEV_serial_data
{

    /** 7. */
    DEV_SER_DATA_7,

    /** 8. */
    DEV_SER_DATA_8
};

/** Stop bits. */
enum DEV_serial_stop
{

    /** 1. */
    DEV_SER_STOP_1,

    /** 2. */
    DEV_SER_STOP_2
};

/** Parity bit. */
enum DEV_serial_parity
{

    /** None parity. */
    DEV_SER_PARITY_NONE,

    /** Even parity. */
    DEV_SER_PARITY_EVEN,

    /** Odd parity. */
    DEV_SER_PARITY_ODD
};

/**
 * Serial Settings Struct.
 *
 */
struct DEV_serial_settings
{

    /** Baud Rate.*/
    enum DEV_serial_baud baud;

    /** Data Bits.*/
    enum DEV_serial_data data;

    /** Stop Bits. */
    enum DEV_serial_stop stop;

    /** Parity Bit. */
    enum DEV_serial_parity parity;

    /** Timeout. */
    int timeout;
};

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize serial settings struct.
 *
 * Set to default values:
 * 9600,8,1,none ; DEV_SER_RX_TIMEOUT.
 *
 * @param settings : Pointer to settings struct.
 */
extern void DEV_serial_settings_init (struct DEV_serial_settings *settings);

/**
 * Print serial settings struct to stdio.
 *
 * Set to default values:
 * 9600,8,1,none ; DEV_SER_RX_TIMEOUT.
 *
 * @param settings : Pointer to settings struct.
 */
extern void DEV_serial_settings_print (const struct DEV_serial_settings *settings);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
