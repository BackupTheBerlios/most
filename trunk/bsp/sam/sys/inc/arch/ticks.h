/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef SAM_TICKS_H
#define SAM_TICKS_H

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
#define SAM_TICKS_IN_HZ     USO_TICKS_10KHZ


/**
 * Initialize interrupt to generate ticks.
 */
extern void SAM_ticks_init (void);

/**
 *
 */
extern void SAM_ticks_interrupt (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void SAM_ticks_debug (void);

/** @}
 */

/** @}
 */


#endif
