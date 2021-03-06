/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_INIT_H
#define MDC_INIT_H

#include <ace/stdio.h>

/** @addtogroup init
 *
 * @{
 */

/** @defgroup mdc_init init.h
 *
 * Initialize Function Interface
 *
 * @{
 */


/**
 * This function is called from the cpu reset vector routine after the stack is initialized.
 * This is the right place to initialize your system.
 */
extern void MDC_init (void);

/** @}
 */

/** @}
 */

#endif
