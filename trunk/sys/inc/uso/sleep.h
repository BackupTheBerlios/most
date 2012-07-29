/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_SLEEP_H
#define USO_SLEEP_H

/** @addtogroup uso
 * @{
 */

/** @defgroup sleep sleep.h
 *
 * A thread will block for a amount of time.
 *
 * @{
 */

/** Select one of this values */
#define USO_TICKS_100HZ     100L /**< System is less responsible, but with less scheduling overhead */
#define USO_TICKS_1KHZ     1000L /**< Default value */
#define USO_TICKS_10KHZ   10000L /**< System is more responsible, but with more scheduling overhead */

/**
 * Global variable defining ticks needed for one sec.
 * The correct value for this depends on the BSP.
 */
extern long USO_ticks_per_sec;

/**
 * Convert msec to ticks.
 */
#define USO_MSEC_2_TICKS(USO_msec) ( (USO_ticks_per_sec * (USO_msec)) / 1000L  )

/**
 * Convert ticks to msec.
 */
#define USO_TICKS_2_MSEC(USO_ticks) ( (1000L * (USO_ticks)) / USO_ticks_per_sec )


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

/** @}
 */

#endif
