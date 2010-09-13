/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_DIGIO_H
#define DEV_DIGIO_H

/** @defgroup digio digio.h
 *
 * Digital IO.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** IO State. */
enum DEV_digio_state
{
	DEV_DIGIO_LOW, /**< Physically low signal. */
    DEV_DIGIO_HIGH /**< Physically high signal. */
};

/** IO logic. */
enum DEV_digio_logig
{
    DEV_DIGIO_POS,  /**< Logically high is at high voltage. */
    DEV_DIGIO_NEG   /**< Logically high is at low voltage. */
};

/** IO edge. */
enum DEV_digio_edge
{
    DEV_DIGIO_NO,     /**< No edge. */
    DEV_DIGIO_RAISE,  /**< Raising edge. */
    DEV_DIGIO_FALL    /**< Falling edge. */
};

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
