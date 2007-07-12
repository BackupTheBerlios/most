/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef USO_BARRIER_H
#define USO_BARRIER_H

#include <ace/stddef.h>
#include "uso/list.h"

/** @defgroup barrier barrier.h
 *
 * Barrier.
 *		
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Barrier struct.
 *
 * Private
 */
struct USO_barrier
{
    /*
     * List of blocked threads 
     */
    USO_list_t threads;
};

/** 
 * Barrier type.
 */
typedef struct USO_barrier USO_barrier_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a barrier.
 *
 * @param barrier : Pointer to barrier.
 */
extern void USO_barrier_init (USO_barrier_t * barrier);

extern void USO_block (USO_barrier_t * barrier);
extern void USO_go (USO_barrier_t * barrier, bool_t all);


/*------------------------------------------------------------------------*/

/** @}
 */

#endif
