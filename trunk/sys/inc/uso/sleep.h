/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_SLEEP_H
#define USO_SLEEP_H

#include <ace/arch/cpu.h>

/** @defgroup sleep sleep.h
 *
 * process timing
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
 * This is normally done by the hardware.
 *
 * If there is a thread waiting and ready to be woken up,
 * it will be put to ready queue.
 */
extern void USO_wakeup (void);

/** 
 * Block caller n ticks long.
 * 
 * @param ticks : block n ticks 
 */
extern void USO_sleep (long ticks);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
