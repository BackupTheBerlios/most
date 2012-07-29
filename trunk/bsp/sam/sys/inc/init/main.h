/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef SAM_MAIN_H
#define SAM_MAIN_H

/** @addtogroup init
 *
 * @{
 */

/** @defgroup main main.h
 *
 * Main Function Interface
 *
 * @{
 */

#define SAM_BOARD "SAM"

/**
 * This function is called from the Init Thread.
 * This is the right place to start your application.
 * Keep in mind that you are running on the init thread.
 * If the function ends, the init thread terminates and
 * its recources are freed.
 */
extern void SAM_main (void);

/** @}
 */

/** @}
 */

#endif
