/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_MUTEX_H
#define USO_MUTEX_H

#include "uso/thread.h"

/** @addtogroup uso
 * @{
 */

/** @defgroup mutex mutex.h
 *
 * Mutual exclusion or also called critical section.
 * A difference to a counting semaphore is that a
 * mutex is owned by a thread which can enter the mutex again.
 *
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Mutex struct.
 *
 * Private
 */
struct USO_mutex
{
    USO_thread_t *lock;         /* Thread which owns the mutex */
    USO_list_t threads;         /* List of blocked threads */
};

typedef struct USO_mutex USO_mutex_t;

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a mutex.
 *
 * @param mutex : Pointer to mutex.
 */
extern void USO_mutex_init (USO_mutex_t * mutex);

/** Lock a mutex. */
extern void USO_lock (USO_mutex_t * mutex);

/** Unlock a Mutex. */
extern void USO_unlock (USO_mutex_t * mutex);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
