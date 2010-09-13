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
    USO_list_t threads; /* List of blocked threads */
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

/**
 * Block on a barrier.
 *
 * @param barrier : Pointer to barrier.
 */
extern void USO_block (USO_barrier_t * barrier);

/**
 * The first thread which is blocking on a barrier is allowed to run.
 *
 * @param barrier : Pointer to barrier.
 */
extern void USO_go (USO_barrier_t * barrier);

/**
 * All threads which are blocking on a barrier are allowed tu run.
 *
 * @param barrier : Pointer to barrier.
 */
extern void USO_go_all (USO_barrier_t * barrier);


/*------------------------------------------------------------------------*/

/** @}
 */

#endif
