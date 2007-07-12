/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_MAIN_H
#define MDC_MAIN_H

/** @addtogroup initialization
 *
 * @{
 */

/** @defgroup main main.h
 *
 * Main Function Interface
 *
 * @{
 */


/**
 * This function is called from the Init Thread.
 * This is the right place to start your application.
 * Keep in mind that you are running on the init thread.
 * If the function ends, the init thread terminates and
 * its recources are freed.
 */
extern void MDC_main (void);

/** @}
 */

/** @}
 */

#endif
