/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_TICKS_H
#define MDC_TICKS_H

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
#define MDC_TICKS_IN_HZ     USO_TICKS_1KHZ


/**
 * Initialize interrupt to generate ticks.
 */
extern void MDC_ticks_init (void);

/** @}
 */

/** @}
 */


#endif
