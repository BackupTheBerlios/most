/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_INIT_H
#define LA2_INIT_H

/** @addtogroup init
 *
 * @{
 */

/** @defgroup init2 init.h
 *
 * Initialize Function Interface
 *
 */

/** @{
 */

/**
 * This function is called from the cpu reset vector routine after the stack is initialized.
 * This is the right place to initialize your system.
 */
extern void LA2_init (void);

/** @}
 */

/** @}
 */

#endif
