/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_SLEEP_H
#define USO_SLEEP_H

#include <ace/arch/cpu.h>

/** @defgroup sleep sleep.h
 *
 * A thread will block for a amount of time.
 *
 * @{
 */

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize sleep functionality.
 *
 */
extern void USO_sleep_init (void);

/**
 * Function which has to be called every tick.
 *
 * During an Interrupt, other Interrupts have to be disabled!
 *
 * If there is a thread waiting and ready to be waken up,
 * it will be put to ready queue.
 */
extern void USO_wakeup (void);

/** 
 * Block caller n ticks long.
 * 
 * @param ticks : n(ticks) ticks.
 */
extern void USO_sleep (long ticks);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
