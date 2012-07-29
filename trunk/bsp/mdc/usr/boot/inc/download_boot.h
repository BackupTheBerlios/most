/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_DOWNLOAD_BOOT_H
#define MDC_DOWNLOAD_BOOT_H

#include <mfs/directory.h>
#include <mfs/sysfs.h>

/** @addtogroup putboot
 *
 * @{
 */

/** @defgroup download_boot download_boot.h
 *
 * Download Bootloader
 *
 * @{
 */

/**
 * Install bootloader download commands.
 */
extern void MDC_download_boot_install (MFS_descriptor_t * boot);

/** @}
 */

/** @}
 */

#endif
