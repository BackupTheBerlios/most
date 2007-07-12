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

/** IO State */
enum DEV_digio_state
{
    /** state low */
    DEV_DIGIO_LOW,

    /** state high */
    DEV_DIGIO_HIGH
};

/** IO logic */
enum DEV_digio_logig
{

    /** pos logigc */
    DEV_DIGIO_POS,

    /** neg logigc */
    DEV_DIGIO_NEG
};

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
