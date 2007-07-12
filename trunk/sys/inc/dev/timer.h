/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_TIMER_H
#define DEV_TIMER_H

#include <ace/arch/cpu.h>
#include <uso/list.h>

/** @defgroup timer timer.h
 *
 * Timer functions.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Timer state 
 */
enum DEV_timer_state
{
    DEV_TIMER_OFF,
    DEV_TIMER_ON
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
};

/** 
 * Timer type.
 *
 */
typedef struct DEV_timer DEV_timer_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize timer functionallity.
 *
 */
extern void DEV_timers_init (void);

/**
 * Initialize an timer.
 *
 * @param timer : Pointer to timer.
 * @param f : Function which has to be called from the timer thread(system).
 * @param param : Parameter which will be passed to f..
 * @param ticks : System ticks you want to wait.
 */
extern void DEV_timer_init (DEV_timer_t * timer,
                            void (*f) (void *), void *param, long ticks);

/**
 * Start timer.
 *
 * @param timer : Pointer to timer.
 */
extern void DEV_timer_start (DEV_timer_t * timer);

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
 * This is normally done by the hardware.
 *
 * If there is a timer in the delta list, it decrements its delta.
 * All timers in the delta list whose delta <= 0, execute ther callback
 * function whith passing param to it.
 *
 * if timer_fire is called in an interrupt(normally),
 * the callback function f runs also in the context of the interrupt.
 */
extern void DEV_timer_fire (void);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
