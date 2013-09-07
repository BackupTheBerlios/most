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

/* Bootloader: 80K 0x100000 - 0x114000, Application : 176K 0x114000 - 0x140000 */
#define SAM_FLASH_START       ((unsigned char *)0x100000)
#define SAM_BOOT_SIZE         ((unsigned long)0x014000)
#define SAM_APPL_SIZE         ((unsigned long)0x02C000)

#define SAM_BOOT_START        SAM_FLASH_START
#define SAM_BOOT_END          (SAM_BOOT_START + SAM_BOOT_SIZE)
#define SAM_APPL_START        (SAM_BOOT_END)
#define SAM_APPL_END          (SAM_APPL_START + SAM_APPL_SIZE)


/**
 * Install application download commands.
 */
extern void SAM_download_install (MFS_descriptor_t * dir, unsigned char *start, unsigned char *end);

/** @}
 */

/** @}
 */

#endif
