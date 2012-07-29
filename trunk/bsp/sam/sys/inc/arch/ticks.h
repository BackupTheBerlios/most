/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef SAM_TICKS_H
#define SAM_TICKS_H

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
#define SAM_TICKS_IN_HZ     USO_TICKS_1KHZ

/**
 * Convert ticks to ticks, done by preprocessor if SAM_msec is const.
 */
#define SAM_MSEC_2_TICKS(SAM_msec) ( (SAM_TICKS_IN_HZ * (SAM_msec)) / 1000L  )

/**
 * Convert ticks to msec, done by preprocessor if SAM_ticks is const.
 */
#define SAM_TICKS_2_MSEC(SAM_ticks) ( (1000L * (SAM_ticks)) / SAM_TICKS_IN_HZ )

/**
 * Initialize interrupt to generate ticks.
 */
extern void SAM_ticks_init (void);

/**
 *
 */
extern void SAM_ticks_interrupt (void);

/**
 *
 */
extern void SAM_ticks_debug (void);

/** @}
 */

/** @}
 */


#endif
