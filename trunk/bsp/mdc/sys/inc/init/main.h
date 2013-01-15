/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_MAIN_H
#define MDC_MAIN_H

#include <ace/version.h>

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

#define MDC_BOARD "MDC"

/**
 * This function is called from the Start Thread.
 * This is the right place to start your application.
 * Keep in mind that you are running on the start thread.
 * If the function ends, the start thread terminates and
 * its recources are freed.
 */
extern void MDC_main (void);

/** @}
 */

/** @}
 */

#endif
