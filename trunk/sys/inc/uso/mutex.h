/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_MUTEX_H
#define USO_MUTEX_H

#include "uso/semaphore.h"

/** @defgroup mutex mutex.h
 *
 * Mutual exclusion or also called critical section.
 *
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** Lock a Mutex */
#define USO_lock USO_wait
/** Unlock a Mutex */
#define USO_unlock USO_signal

/** Mutex is a semaphore initialized with 1 */
typedef USO_semaphore_t USO_mutex_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a mutex.
 *
 * A mutex is a semaphor initialized with 1
 * @param mutex : Pointer to mutex.
 */
extern void USO_mutex_init (USO_mutex_t * mutex);

/*------------------------------------------------------------------------*/

/** @}
 */

 #endif
