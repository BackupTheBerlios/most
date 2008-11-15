/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_INIT_H
#define SAM_INIT_H

/** @addtogroup initialization
 *
 * @{
 */

/** @defgroup init init.h
 *
 * Initialize Function Interface
 *
 * @{
 */

/**
 * Serial communication Interface.
 */
extern FILE *ser0;

/**
 * This function is called from the cpu reset vector routine after the stack is initialized.
 * This is the right place to initialize your system.
 */
extern void SAM_init (void);

/** @}
 */

/** @}
 */

#endif
