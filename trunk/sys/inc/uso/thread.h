/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_THREAD_H
#define USO_THREAD_H

#include <ace/stddef.h>

#include "uso/list.h"
#include "uso/arch/cpu.h"
#include "ace/stdio.h"
#include "mfs/vfs.h"

/** @defgroup thread thread.h
 *
 * Multithreading.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** 
 * Thread state.
 *
 * A thread can be in one of this states.
 * 
 * RUNNING: The current thread which is executed.
 * READY: Threads which are ready for run.
 * BLOCKED: Threads which are blocked.
 * INIT: Initialized but not started.
 */

enum USO_thread_state
{
    USO_INIT,
    USO_READY,
    USO_RUNNING,
    USO_BLOCKED_WAIT,
    USO_BLOCKED_SLEEP,
    USO_BLOCKED_SEND,
    USO_BLOCKED_RECEIVE,
    USO_BLOCKED_REPLY,
    USO_BLOCKED_BLOCK,
    USO_BLOCKED_LOCK,
    USO_BLOCKED_CATCH,
    USO_EXIT,
    USO_DEAD
};

/** 
 * Thread type.
 *
 * There are two queues for ready threads.
 *
 * INTERRUPT:
 * If there is any interrupt thread ready, it is executed emediatly and
 * befor any ready system and user thread. So interrupt threads must
 * block and not make a yield. 
 * 
 * SYSTEM:
 * If there is any system thread ready, it is executed 
 * befor any ready user thread. So system threads must
 * block and not make a yield.
 * 
 * USER:
 * Ready User threads are executed if there is no system thread.
 * They can do a yield.
 *
 * The one and only IDLE thread is created when you call the function
 * USO_transform(). 
 * You should never use IDLE!
 */

enum USO_thread_priority
{
    USO_IDLE,
    USO_USER,
    USO_SYSTEM,
    USO_INTERRUPT
};

/**
 * FIFO:
 * Thread runs until int blocks, make a yield or is interrupted by a thread 
 * with priority INTERRUPT.
 * 
 * ROUND_ROBIN:
 * Same like FIFO but with preemption, each thread has its time slice.
 * 
 */
enum USO_thread_scheduling
{
    USO_FIFO,
    USO_ROUND_ROBIN
};


enum USO_thread_flags
{
    USO_FLAG_DETACH,
    USO_FLAG_FREE_ARG
};

/*
 * Thread struct.
 *
 * Private.
 */

struct USO_thread
{
    USO_node_t node;
    USO_cpu_t cpu _PACKED_;
    enum USO_thread_state state;
    enum USO_thread_priority priority;
    enum USO_thread_scheduling scheduling;
    void (*enter) (void *);
    void (*cleanup) (void);
    void *arg;
    bool_t stop;
	u32_t flags;
	u32_t signals;
    FILE *in;
    FILE *out;
    USO_stack_t *stack;
    USO_stack_t *stack_bot;
    USO_stack_t *stack_top;
    USO_stack_t *stack_max;
    void * message;
	unsigned long ticks;
	MFS_descriptor_t *desc;
};

/** 
 * Thread type.
 *
 */

typedef struct USO_thread USO_thread_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a thread.
 *
 * @param thread : Pointer to thread.
 * @param enter : Entry point for the thread,
 *                thread execution will start there.
 * @param stack : Pointer to stack which should be used.
 * @param stack_size : Size of the stack.
 * @param priority : USO_SYSTEM for system threads,
 *               which should allways block. 
 *               USO_USER for user threads,
 *               which can do a yield.
 * @param scheduling : Scheduling algorithm.
 * @param name : Name for the thread.
 */
extern void USO_thread_init (USO_thread_t * thread,
                             void (*enter) (void *),
                             USO_stack_t * stack,
                             int stack_size,
                             enum USO_thread_priority priority,
                             enum USO_thread_scheduling scheduling,
                             char *name);

/**
 * Create a thread.
 *
 * @param enter : Entry point for the thread,
 *                thread execution will start there.
 * @param stack_size : Size of the stack.
 * @param priority : USO_SYSTEM for system threads,
 *               which should allways block. 
 *               USO_USER for user threads,
 *               which can do a yield.
 * @param scheduling : Scheduling algorithm.
 * @param name : Name for the thread.
 * @param detach : TRUE for free Resources(Stack) and remove from file system.
 * @return : Pointer to thread.
 */
extern USO_thread_t *USO_thread_new (void (*enter) (void *),
                                     int stack_size,
                                     enum USO_thread_priority priority,
		                             enum USO_thread_scheduling scheduling,
                                     char *name);

/**
 * Terminate thread, called by scheduler.
 *
 *@param thread : Poiner to thread.
 */
extern void USO_thread_terminate (USO_thread_t * thread);

/**
 * Overwrite IO Streams for thread.
 *
 * IO Streams inherited by the parent thread.
 * IO Streams from the Idle thread are NULL.
 *
 * @param thread : Pointer to thread.
 * @param in : Input stream.
 * @param out : Output stream.
 */
extern void USO_thread_ios_init (USO_thread_t * thread,
                                 FILE * in, FILE * out);


/**
 * Set argument.
 *
 * @param thread : thread.
 * @param arg : argument.
 * @param free_arg : Free argument wenn thread is deleted.
 */
extern void USO_thread_arg_init (USO_thread_t * thread, void * arg);

extern void USO_thread_flags_set(USO_thread_t * thread, u32_t flags);

extern void USO_cleanup_install(void (*cleanup) (void));

/**
 * Start thread.
 * 
 * @param thread : Pointer to thread.
 */
extern void USO_start (USO_thread_t * thread);

/**
 * Stop thread.
 * 
 * @param thread : Pointer to thread.
 */
extern void USO_stop (USO_thread_t * thread);

extern void USO_raise(USO_thread_t * thread, u32_t signals);

extern u32_t USO_catch(void);

extern void USO_exit (void);

/**
 * Give other threads a chance to run.
 */
extern void USO_yield (void);

/**
 * Print message and wait for the watchdog.
 */
extern void USO_panic (char *file, int line);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
