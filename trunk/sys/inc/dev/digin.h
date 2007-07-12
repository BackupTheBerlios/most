/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_DIGIN_H
#define DEV_DIGIN_H

#include <uso/list.h>
#include <uso/semaphore.h>

#include "dev/digio.h"

/** @defgroup digin digin.h
 *
 * Digital Input.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Struct digital input Private 
 */
struct DEV_digin
{
    USO_node_t node;
    enum DEV_digio_state state;
    enum DEV_digio_logig logig;
    unsigned long (*sample) (void);
    int debounce_time;
    int debounce_count;
    int pos_edge_count;
    int neg_edge_count;
    USO_semaphore_t pos_edge_sem;
    USO_semaphore_t neg_edge_sem;
};

/** Digital input type */
typedef struct DEV_digin DEV_digin_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize digital input.
 *
 * @param in : Pointer to digital input type.
 * @param logig : pos or neg logic
 * @param sample : Function to sample the input.
 * @param debounce_time : Entprell zeit.
 */
extern void DEV_digin_init (DEV_digin_t * in,
                            enum DEV_digio_logig logig,
                            unsigned long (*sample) (void), int debounce_time);

/**
 * Sample all digital inputs.
 *
 * Calls each sample function of all dig inputs.
 * Should be periodically called from an inerrupt.
 */
extern void DEV_digin_sample (void);

/**
 * Determine if input is set.
 *
 * @param in : Pointer to dig input.
 *
 * @return TRUE if logical set elde FALSE.
 */
extern bool_t DEV_digin_isset (DEV_digin_t * in);

/**
 * Wait for input.
 *
 * @param in : Pointer to dig input.
 * @param state : logigal state.
 * @param level : FALSE flanken , TRUE pegel getriggert.
 */
extern void DEV_digin_wait (DEV_digin_t * in,
                            enum DEV_digio_state state, bool_t level);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
