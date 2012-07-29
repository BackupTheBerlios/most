/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_SEMAPHORE_H
#define USO_SEMAPHORE_H

#include "uso/list.h"

/** @addtogroup uso
 * @{
 */

/** @defgroup semaphore semaphore.h
 *
 * Counting semaphore.
 * A difference to a mutex is that if a thread reenter a critical section
 * inside the critical section, it will block.
 * If you want to do this you have to use the USO_mutex_t.
 *
 * Wait and signal keep the following condition invariant:
 *
 * A non negative semaphore count means that the queue is empty.
 * A semaphore count of negative n means that the queue contains n 
 * waiting threads.
 *
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Semaphore struct.
 *
 * Private
 */
struct USO_semaphore
{
    /* -n : n threads in the list n : counting n wait calls before block */
    int count;

    /* List of blocked threads */
    USO_list_t threads;
};

/** 
 * Semaphore type.
 */
typedef struct USO_semaphore USO_semaphore_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a semaphore.
 *
 * @param semaphore : Pointer to semaphore.
 * @param count : For a signal semaphore use 0,
 *                for a mutex use 1,
 *                for a count semaphore use any positive integer.
 */
extern void USO_semaphore_init (USO_semaphore_t * semaphore, int count);

/**
 * Wait for a semaphore.
 *
 * If count <= 0 the thread blocks on the semaphore.
 *
 * @param semaphore : Pointer to semaphore.
 */
extern void USO_wait (USO_semaphore_t * semaphore);

/**
 * Signal on a semaphore.
 *
 * If there are waiting threads, the first is wakedup.
 *
 * @param semaphore : Pointer to semaphore.
 */
extern void USO_signal (USO_semaphore_t * semaphore);

/**
 * Get semaphore count.
 *
 * @param semaphore : Pointer to semaphore
 * return Count
 */
extern int USO_sem_count (USO_semaphore_t * semaphore);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
