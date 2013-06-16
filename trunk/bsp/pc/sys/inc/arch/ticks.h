/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef PC_TICKS_H
#define PC_TICKS_H

#include <ace/stddef.h>
#include <uso/sleep.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup ticks ticks.h
 *
 * Ticks.
 *
 * @{
 */


/**
 * Hartbeat
 */
#define PC_TICKS_IN_HZ     USO_TICKS_1KHZ


/**
 * Initialize interrupt to generate ticks.
 */
extern void PC_ticks_init (void);

/**
 *
 */
extern void PC_ticks_interrupt (void);


/** @}
 */

/** @}
 */


#endif
