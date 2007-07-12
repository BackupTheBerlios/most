/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_DIGOUT_H
#define DEV_DIGOUT_H

#include "dev/digio.h"

/** @defgroup digout digout.h
 *
 * Digital Output.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Struct digital output Private 
 */
struct DEV_digout
{
    enum DEV_digio_state state;
    enum DEV_digio_logig logig;
    void (*set) (void);
    void (*clear) (void);
};

/** Digital output type */
typedef struct DEV_digout DEV_digout_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Init a digital output.
 *
 * @param out : Pointer to output.
 * @param state : set the output to hi or low.
 * @param logig : pos or neg logic.
 * @param set : Function which will be called to set the output.
 * @param clear : Function which will be called to clear the output.
 */
extern void DEV_digout_init (DEV_digout_t * out,
                             enum DEV_digio_state state,
                             enum DEV_digio_logig logig,
                             void (*set) (void), void (*clear) (void));

/**
 * Set output.
 *
 * Calls the set function for the output.
 *
 * @param out : Pointer to output.
 */
extern void DEV_digout_set (DEV_digout_t * out);

/**
 * Clear output.
 *
 * Calls the clear function for the output.
 *
 * @param out : Pointer to output.
 */
extern void DEV_digout_clear (DEV_digout_t * out);

/**
 * Toggle output.
 *
 * @param out : Pointer to output.
 */
extern void DEV_digout_toggle (DEV_digout_t * out);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
