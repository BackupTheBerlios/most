/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef PC_MAIN_H
#define PC_MAIN_H

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

#define PC_BOARD "PC"

/**
 * This function is called from the Start Thread.
 * This is the right place to start your application.
 * Keep in mind that you are running on the start thread.
 * If the function ends, the start thread terminates and
 * its recources are freed.
 */
extern void PC_main (void);

/** @}
 */

/** @}
 */

#endif
