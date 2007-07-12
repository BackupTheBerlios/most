/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_SERIAL_INT_INTERFACE_H
#define DEV_SERIAL_INT_INTERFACE_H

#include <ace/stddef.h>

#include "dev/serial_error.h"

/** @defgroup serial_int_interface serial_int_interface.h
 *
 * Interface to interrupt routines (lower level).
 * Functions which will be called by the interrupt routines.
 * Each Serial Device Type has to implement this interface
 * so that it can be casted to that interface type.
 * First Entry in the device struct.
 * @{
 */

/*------------- Representation ------------------------------------------*/


/** Serial device type. */
typedef struct DEV_serial DEV_serial_t;


/** Serial interrupt Interface Type (lower level). */
typedef struct DEV_serial_int_interface DEV_serial_int_interface_t;

/**
 * Interface Struct.
 */
struct DEV_serial_int_interface
{

    /** Function called by the interrupt routines when a char is received */
    void (*rx_char) (DEV_serial_t *, char);

    /** Function called by the interrupt routnines when e rx error occours */
    void (*rx_error) (DEV_serial_t *, enum DEV_serial_rx_err);

    /** Function called by the interrupt routines to fetch a character 
     * for transmission, if non is available EOF is returned.
     */
    int (*tx_char) (DEV_serial_t *);

    /** Function called by the inerrupt routines if transmission is finished */
    void (*tx_finished) (DEV_serial_t *);
};

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
