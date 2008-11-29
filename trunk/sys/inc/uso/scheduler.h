/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_SCHEDULER_H
#define USO_SCHEDULER_H

#include "uso/thread.h"

/** @defgroup scheduler scheduler.h
 *
 * Thread scheduler.
 *
 * The scheduler is cooperative.
 * This will make it useable also on not reentrant code!
 * 
 * Until you call USO_transform, interrupts should be disabled!
 * USO_transform enables all interrupts!
 * System calles are disable and restore interrupts!
 *
 * @{
 */

/*-------------- Interface -----------------------------------------------*/

#define USO_SCHEDULE_TIME_MSEC 100 

/** Current thread */
extern USO_thread_t *USO_current(void);

/**
 * Go multithreading!
 *
 * Call this function once if you are ready for it!
 * The transform function does not return!
 * It will end up in the idle thread!
 * You will be called back with the init function,
 * which you have to implement!
 *
 * @param init : Callback function.
 */
extern void USO_transform (void (*init) (void),
	USO_stack_t* stack, int stack_size);

/**
 * Determine the next thread which is ready to run. 
 */
extern USO_thread_t *USO_next2run (void);

/**
 * Make a new thread running.
 * Call the exit function of a thread which has finished. 
 */
extern void USO_schedule (USO_thread_t *);

/**
 * Brings a thread to its ready queue.
 */
extern void USO_ready (USO_thread_t *);

/**
 * Can be called by the timer interupt (ticks).
 */
extern void USO_preempt(void);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
