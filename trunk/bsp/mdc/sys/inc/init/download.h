/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_DOWNLOAD_APP_H
#define MDC_DOWNLOAD_APP_H

#include <mfs/directory.h>
#include <mfs/sysfs.h>

/** @addtogroup boot
 *
 * @{
 */

/** @defgroup download_app download_app.h
 *
 * Download Application
 *
 * @{
 */


#define MDC_PUTBOOT_START 0
#define MDC_BOOT_START    1
#define MDC_APPL_START    2
#define MDC_APPL_END      8

/**
 * Install application download commands.
 */
extern void MDC_download_install (MFS_descriptor_t * dir, unsigned long start, unsigned long end);

/** @}
 */

/** @}
 */

#endif
