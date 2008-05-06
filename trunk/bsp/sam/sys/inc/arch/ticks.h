/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef SAM_TICKS_H
#define SAM_TICKS_H

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
 * Initialize interrupt to generate ticks.
 */
extern void SAM_ticks_init (void);

extern void SAM_ticks_interrupt (void);

/** @}
 */

/** @}
 */


#endif
