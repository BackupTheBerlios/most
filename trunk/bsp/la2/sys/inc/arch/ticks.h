/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef LA2_TICKS_H
#define LA2_TICKS_H

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
#define LA2_TICKS_IN_HZ     USO_TICKS_1KHZ


/**
 * Initialize interrupt to generate ticks.
 */
extern void LA2_ticks_init (void);

/**
 *
 */
extern void LA2_ticks_interrupt (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_ticks_debug (void);

/** @}
 */

/** @}
 */


#endif
