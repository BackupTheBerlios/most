/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_SERIAL_B_H
#define DEV_SERIAL_B_H

#include <ace/stddef.h>
#include <uso/pipe.h>
#include <uso/mutex.h>
#include <uso/barrier.h>
#include <mfs/vfs.h>
#include <mfs/stream.h>

#include <dev/timer.h>
#include <dev/serial_error.h>
#include <dev/serial_settings.h>
#include <dev/serial_int_interface.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup serial serial.h
 *
 * Serial Device.
 * Blocking functions.
 * @{
 */

/*------------- Representation ------------------------------------------*/

enum DEV_ser_control_key
{
    DEV_SER_CTRL_RX_TIMEOUT              /**< Set serial receive timeout in sec. */
};

/*
 * Serial Struct
 * 
 * Private.
 */
struct DEV_serial
{
    const DEV_serial_int_interface_t *int_interface;
    USO_pipe_t rx_buf;
    USO_pipe_t tx_buf;
    ACE_bool_t tx_busy;
    void (*open) (const struct DEV_serial_settings *);
    void (*close) (void);
    void (*tx_start) (void);
    struct DEV_serial_error error;
    const struct DEV_serial_settings *settings;
    char rx_buffer[DEV_SERIAL_B_RX_BUFFER_SIZE];
    char tx_buffer[DEV_SERIAL_B_TX_BUFFER_SIZE];
    USO_mutex_t rx_mutex;
    USO_mutex_t tx_mutex;
    USO_barrier_t rx_barrier;
    ACE_bool_t block;
    ACE_bool_t rx_cancel;
    DEV_timer_t rx_timer;
    long rx_timeout_sec;
};


/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a serial device.
 *
 * @param serial : Pointer serial device.
 * @param settings : Pointer serial device.
 * @param open : low level function for opening a device.
 * @param close : low level function for closing a device.
 * @param tx_start : low level function for starting transmission.
 * @param block : TRUE for blocking, FALSE for non blocking.
 * @param name : name for device.
 */
extern void DEV_serial_init (DEV_serial_t * serial,
                             const struct DEV_serial_settings *settings,
                             void (*open) (const struct DEV_serial_settings *),
                             void (*close) (void),
                             void (*tx_start) (void), ACE_bool_t block, char *name);


extern void DEV_serial_rx_cancel(DEV_serial_t *serial);

/*------------------------------------------------------------------------*/


/** @}
 */

/** @}
 */

#endif
