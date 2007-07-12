/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_BOOT_H
#define MDC_BOOT_H

/** @addtogroup initialization
 *
 * @{
 */

/** @defgroup jump jump.h
 *
 * Jump to Bootloader or Application
 *
 * @{
 */

extern MFS_descriptor_t *boot;

/**
 * Install jump commands.
 */
extern void MDC_boot_install (void);

/** @}
 */

/** @}
 */

#endif
