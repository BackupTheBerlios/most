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
 * The scheduler supports two scheduling algorithms.
 * FIFO: A thread runs until a thread with  priority INTERRUPT get ready,
 *       or it blocks on any synchronization mechanism, or it yields.
 * Round Robin: Works like FIFO but the threads are also interrupted if a time slice has expired.
 *              This means it is FIFO with preemtion.
 * 
 * Until you call USO_transform, interrupts should be disabled!
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
 * Call this function once if you are ready for it
 * (basic initialization, cpu, heap, stack, .. is done).
 * The transform function does not return!
 * It will end up in the idle thread!
 * You will be called back with the init function, where you can do more
 * initialization and start your threads.
 *
 * @param init : Callback function.
 * @param stack : Stack for the idle thread.
 * @param stack_size : Stack size.
 */
extern void USO_transform (void (*init) (void),
	USO_stack_t* stack, int stack_size);

/**
 * Determine the next thread which is ready to run.
 * Don't use this function outside the kernel.
 */
extern USO_thread_t *USO_next2run (void);

/**
 * Make a new thread running.
 * Call the exit function of a thread which has finished.
 * Don't use this function outside the kernel.
 */
extern void USO_schedule (USO_thread_t *);

/**
 * Put thread to the ready queue.
 * Don't use this function outside the kernel.
 */
extern void USO_ready (USO_thread_t *);

/**
 * Do preemtion.
 * May be called by the timer interrupt (ticks).
 * If the time slice of a thread is expired it gives control
 * to the next ready thread.
 */
extern void USO_preempt(void);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
