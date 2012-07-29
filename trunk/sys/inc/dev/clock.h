#ifndef DEV_CLOCK_H
#define DEV_CLOCK_H

/** @addtogroup dev
 * @{
 */


/** @defgroup clock clock.h
 *
 * Clock functions.
 * @{
 */

/**
 * Initialize clock.
 */
extern void DEV_clock_init (unsigned long (*get_usec) (void));

/**
 * This function shall be called from the timer interrupt.
 */
extern void DEV_clock_tick (void);

/**
 * Get current clock ticks.
 *
 * @return Current clock ticks.
 */
extern unsigned long DEV_get_ticks (void);

/**
 * Get the difference from the current clock ticks
 * to an older value of get_clocks_ticks.
 *
 * @param old_ticks : Old value of clock ticks.
 * @return The difference to the current value of clock ticks.
 */
extern unsigned long DEV_get_ticks_diff (unsigned long old_ticks);

/**
 * Get elapsed us since last clock tick
 * For the case we have missed an tick interrupt, more us elapsed than one tick needs.
 * @return usec since last clock tick.
 */
extern unsigned long DEV_get_usec (void);

/** @}
 */


/** @}
 */

#endif /* DEV_CLOCK_H */
