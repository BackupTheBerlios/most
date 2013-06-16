/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef PC_INIT_H
#define PC_INIT_H

#include <arch/multiboot.h>

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
extern void PC_init_kernel (struct multiboot_info *mb_info);

/** @}
 */

/** @}
 */

#endif
