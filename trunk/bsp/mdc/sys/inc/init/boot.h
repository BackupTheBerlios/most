/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_BOOT_H
#define MDC_BOOT_H

#include <mfs/sysfs.h>

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

extern void MDC_bootp(void);
extern void MDC_start_app(void);
extern void MDC_start_boot(void);

/**
 * Install jump commands.
 */
extern void MDC_boot_install (MFS_descriptor_t *bsp);

/** @}
 */

/** @}
 */

#endif
