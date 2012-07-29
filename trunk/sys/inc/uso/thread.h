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

/** @addtogroup uso
 * @{
 */

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
 */

enum USO_thread_state
{
    USO_INIT,                  /**< Initialized but not started. */
    USO_READY,                 /**< Waiting on the ready queue to became running. */
    USO_RUNNING,               /**< Current active thread. */
    USO_BLOCKED_WAIT,          /**< Thread is waiting for a semaphore. */
    USO_BLOCKED_SLEEP,         /**< Thread is sleeping. */
    USO_BLOCKED_SEND,          /**< Send message to port is waiting until the message can be sent. */
    USO_BLOCKED_RECEIVE,       /**< Receive message from a port is waiting until a message is received. */
    USO_BLOCKED_REPLY,         /**< Send message to a port is waiting until a reply message is received. */
    USO_BLOCKED_BLOCK,         /**< Thread is blocked on a barrier. */
    USO_BLOCKED_LOCK,          /**< Thread is blocked on a mutex. */
    USO_BLOCKED_CATCH,         /**< Thread is waiting for signals. */
    USO_EXIT,                  /**< Thread entry function has finished or the exit function is called. */
    USO_DEAD                   /**< Thread has finished, cleanup is done and allocated resources are freed. */
};

/** 
 * Thread type.
 *
 * There are three queues for ready threads.
 *
 * INTERRUPT:
 * If there is any interrupt thread ready, it is executed immediately and
 * before any ready system and user thread. So interrupt threads must
 * block and they shall not make a yield.
 * The interrupt priority is for fast response to IO events without doing
 * the hole interrupt handling in the interrupt context. The interrupt handler shall pass
 * IO processing as fast as possible to an interrupt thread.
 * This shall be true for block devices but may not for character devices.
 * Execution time of threads with interrupt priority has to be very short.
 * 
 * SYSTEM:
 * If there is any system thread ready, it is executed 
 * before any ready user thread. So system threads must
 * block and they shall not make a yield. The system priority is intended
 * for system services. Normally system threads shall block waiting for some IO.
 *
 * 
 * USER:
 * Ready User threads are executed if there is no system and interrupt thread.
 * They should do a yield if they are FIFO threads and they can run always without
 * blocking for some IO.
 *
 * IDLE:
 * The one and only IDLE thread is created when you call the function
 * USO_transform(). 
 * You should never use IDLE!
 */

enum USO_thread_priority
{
    USO_IDLE,          /**< Idle thread. */
    USO_USER,          /**< User threads. */
    USO_SYSTEM,        /**< System services. */
    USO_INTERRUPT      /**< Fast IO threads. */
};

/**
 * FIFO:
 * Thread runs until it blocks, make a yield or is interrupted by a thread
 * with higher priority.
 * 
 * ROUND_ROBIN:
 * Same like FIFO but with preemption, each thread has its time slice.
 * 
 */
enum USO_thread_scheduling
{
    USO_FIFO,             /**< FIFO scheduling. */
    USO_ROUND_ROBIN       /**< Round Robin scheduling. */
};


/**
 * FLAG_DETACH:
 * Release the resources of an allocated thread which has finished:
 * Remove its file descriptor and release the stack memory.
 *
 * FLAG_FREE_ARG:
 * Release the memory for the thread argument. If you have to pass an argument to an thread
 * in an allocated memory, the thread may release the memory when it has finished.
 *
 */
enum USO_thread_flags
{
    USO_FLAG_DETACH,      /**< Remove thread descriptor and free its resources. */
    USO_FLAG_FREE_ARG     /**< Release the memory for the thread argument. */
};

/*
 * Thread struct.
 *
 * Private.
 */

struct USO_thread
{
    USO_node_t node;
    USO_cpu_t cpu ACE_PACKED_;
    enum USO_thread_state state;
    enum USO_thread_priority priority;
    enum USO_thread_scheduling scheduling;
    void (*enter) (void *);
    void (*cleanup) (void);
    void *arg;
    ACE_bool_t stop;
    ACE_u32_t flags;
    ACE_u32_t signals;
    ACE_FILE *in;
    ACE_FILE *out;
    USO_stack_t *stack;
    int stack_size;
    USO_stack_t *stack_bot;
    USO_stack_t *stack_top;
    USO_stack_t *stack_max;
    void *message;
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
 *                thread execution will start here.
 * @param stack : Pointer to stack which should be used.
 * @param stack_size : Size of the stack.
 * @param priority : USO_SYSTEM for system threads,
 *               USO_USER for user threads,
 *               USO_INTERRUPT for fast IO threads.
 * @param scheduling : Scheduling algorithm USO_FIFO or USO_ROUND_ROBIN.
 * @param name : Name for the thread.
 */
extern void USO_thread_init (USO_thread_t * thread,
                             void (*enter) (void *),
                             USO_stack_t * stack,
                             int stack_size,
                             enum USO_thread_priority priority,
                             enum USO_thread_scheduling scheduling, char *name);

/**
 * Create a thread.
 *
 * @param enter : Entry point for the thread,
 *                thread execution will start there.
 * @param stack_size : Size of the stack.
 * @param priority : USO_SYSTEM for system threads,
 *               USO_USER for user threads,
 *               USO_INTERRUPT for fast IO threads.
 * @param scheduling : Scheduling algorithm USO_FIFO or USO_ROUND_ROBIN.
 * @param name : Name for the thread.
 * @return : Pointer to thread.
 */
extern USO_thread_t *USO_thread_new (void (*enter) (void *),
                                     int stack_size,
                                     enum USO_thread_priority priority,
                                     enum USO_thread_scheduling scheduling, char *name);

/**
 * Terminate thread, called only by scheduler.
 *
 *@param thread : Poiner to thread.
 */
extern void USO_thread_terminate (USO_thread_t * thread);

/**
 * Initialize, overwrite IO Streams for thread.
 *
 * IO Streams inherited by the parent thread.
 * IO Streams from the Idle thread are NULL.
 *
 * @param thread : Pointer to thread.
 * @param in : Input stream.
 * @param out : Output stream.
 */
extern void USO_thread_ios_init (USO_thread_t * thread, ACE_FILE * in, ACE_FILE * out);


/**
 * Set argument.
 *
 * @param thread : thread.
 * @param arg : argument.
 */
extern void USO_thread_arg_init (USO_thread_t * thread, void *arg);

/**
 * Set thread flags.
 *
 * @param thread : thread.
 * @param flags : Set flags (for example: 1 << FLAG_DETACH & 1 << FLAG_ANY).
 */
extern void USO_thread_flags_set (USO_thread_t * thread, ACE_u32_t flags);

/**
 * Install for the current thread a cleanup handler.
 * The cleanup handler will be executed when the thread terminates.
 *
 * @param cleanup : Cleanup handler function.
 */
extern void USO_cleanup_install (void (*cleanup) (void));

/**
 * Start thread.
 * 
 * @param thread : Pointer to thread which has do be started.
 */
extern void USO_start (USO_thread_t * thread);

/**
 * Stop thread.
 * 
 * @param thread : Pointer to thread which has to be stopped.
 */
extern void USO_stop (USO_thread_t * thread);

/**
 * Send signals to a thread.
 *
 * @param thread : thread which may catch the signals.
 * @param signals : The signals will be ored to the already set signals.
 */
extern void USO_raise (USO_thread_t * thread, ACE_u32_t signals);

/**
 * Current thread block until signals are sent.
 * The signals will be reset to 0.
 */
extern ACE_u32_t USO_catch (void);

/**
 * Current thread will exit and finish.
 */
extern void USO_exit (void);

/**
 * Give other threads a chance to run.
 */
extern void USO_yield (void);

/**
 * Return name of current thread.
 */
extern char *USO_thread_name (void);

/**
 * Print thread info header.
 */
extern void USO_thread_info_head (void);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
