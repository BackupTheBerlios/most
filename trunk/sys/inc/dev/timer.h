/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_TIMER_H
#define DEV_TIMER_H

#include <uso/sleep.h>
#include <uso/list.h>
#include <mfs/vfs.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup timer timer.h
 *
 * Timer functions.
 * Currently the timer callback functions run on an interrupt context.
 * Maybe a timer thread shall be used so the timer functions can use synchronization mechanism.
 *
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Timer state 
 */
enum DEV_timer_state
{
    DEV_TIMER_OFF, /**< OFF. */
    DEV_TIMER_ON   /**< ON. */
};

/*
 * Timer context.
 */
enum DEV_timer_ctx
{
    DEV_TIMER_INT,     /**< Timer f is executed at interrupt ctx. */
    DEV_TIMER_THREAD   /**< Timer f is executed at thread ctx. */
};

/*
 * Timer struct.
 *
 * Private
 */
struct DEV_timer
{
    USO_node_t node;
    long ticks;
    void (*f) (void *);
    void *param;
    enum DEV_timer_state state;
    enum DEV_timer_ctx ctx;
    MFS_descriptor_t *desc;
};

/** 
 * Timer type.
 *
 */
typedef struct DEV_timer DEV_timer_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Create and start timer thread.
 *
 */
extern void DEV_timers_start (int timers_stack_size);

/**
 * Initialize timer functionality.
 *
 */
extern void DEV_timers_init (void);

/**
 * Initialize an timer.
 *
 * @param timer : Pointer to timer.
 * @param f : Callback function, currently called at interrupt context.
 * @param param : Parameter which will be passed to function f.
 * @param ticks : System ticks until the timer expires.
 * @param ctx : Context where timer function is executed.
 */
extern void DEV_timer_init (DEV_timer_t * timer,
                            void (*f) (void *), void *param, enum DEV_timer_ctx ctx);

/**
 * Install entry in sysfs.
 *
 * @param timer : Pointer to timer.
 * @param name : Name for entry.
 */
extern void DEV_timer_install (DEV_timer_t * timer, char *name);

/**
 * Remove entry from sysfs.
 *
 * @param timer : Pointer to timer.
 */
extern void DEV_timer_remove (DEV_timer_t * timer);

/**
 * Start timer.
 *
 * @param timer : Pointer to timer.
 */
extern void DEV_timer_start (DEV_timer_t * timer, long ticks);

/**
 * Stop timer.
 *
 * @param timer : Pointer to timer.
 */
extern void DEV_timer_stop (DEV_timer_t * timer);

/**
 * Function which has to be called periodically.
 *
 * During an Interrupt, other Interrupts have to be disabled!
 *
 * If there is a timer in the delta list, it decrements its delta.
 * All timers in the delta list whose delta <= 0, execute their callback
 * function whit passing parameter to it.
 *
 * If timer_fire is called in an interrupt(normally),
 * the callback function f runs also in the context of the interrupt.
 */
extern void DEV_timer_fire (void);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
