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
 * Convert ticks to ticks, done by preprocessor if SAM_msec is const.
 */
#define MDC_MSEC_2_TICKS(MDC_msec) ( (MDC_TICKS_IN_HZ * (MDC_msec)) / 1000L  )

/**
 * Convert ticks to msec, done by preprocessor if SAM_ticks is const.
 */
#define MDC_TICKS_2_MSEC(MDC_ticks) ( (1000L * (MDC_ticks)) / MDC_TICKS_IN_HZ )

/**
 * Initialize interrupt to generate ticks.
 */
extern void MDC_ticks_init (void);

/** @}
 */

/** @}
 */


#endif
