/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_DIGIN_H
#define DEV_DIGIN_H

#include <uso/list.h>

#include "dev/digio.h"

/** @defgroup digin digin.h
 *
 * Digital Input.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/* private */
struct DEV_diginputs
{
	USO_list_t inputs;
};

/**
 * A collection of digital inputs.
 */
typedef struct DEV_diginputs DEV_diginputs_t;

/*
 * Struct digital input.
 * Private.
 */
struct DEV_digin
{
    USO_node_t node;
    enum DEV_digio_state state;
    enum DEV_digio_logig logig;
    enum DEV_digio_edge edge;
    unsigned long (*sample) (void);
    unsigned int debounce_time;
    unsigned int debounce_count;
};

/** Digital input type */
typedef struct DEV_digin DEV_digin_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

extern void DEV_diginputs_init (DEV_diginputs_t* inputs);

/**
 * Initialize digital input.
 *
 * @param in : Pointer to digital input type.
 * @param logig : pos or neg logic
 * @param sample : Function to sample the input.
 * @param debounce_time : Wait for stabilized input signal.
 */
extern void DEV_digin_init (DEV_diginputs_t* inputs,
							DEV_digin_t * in,
                            enum DEV_digio_logig logig,
                            unsigned long (*sample) (void),
                            unsigned int debounce_time);

/**
 * Sample all digital inputs.
 *
 * Calls each sample function of all dig inputs.
 * Should be periodically called from an interrupt.
 */
extern void DEV_diginputs_sample (DEV_diginputs_t* inputs);

/**
 * Determine if input is set.
 *
 * @param in : Pointer to digital input.
 *
 * @return TRUE if logical set else FALSE.
 */
extern ACE_bool_t DEV_digin_isset (DEV_digin_t * in);

/**
 * Check for edges.
 */
extern enum DEV_digio_edge
DEV_digin_ischanged (DEV_digin_t * in);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
