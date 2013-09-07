/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_DOWNLOAD_APP_H
#define LA2_DOWNLOAD_APP_H

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

/* Bootloader: 128K 0x40000000 - 0x40020000, Application : 7*128K = 1M - 128K = 896K, 40020000 - 40100000 */
#define LA2_FLASH_START       ((unsigned char *)0x40000000)
#define LA2_BOOT_SIZE         ((unsigned long)0x020000)
#define LA2_APPL_SIZE         ((unsigned long)0x0E0000)

#define LA2_BOOT_START        LA2_FLASH_START
#define LA2_BOOT_END          (LA2_BOOT_START + LA2_BOOT_SIZE)
#define LA2_APPL_START        (LA2_BOOT_END)
#define LA2_APPL_END          (LA2_APPL_START + LA2_APPL_SIZE)


/**
 * Install application download commands.
 */
extern void LA2_download_install (MFS_descriptor_t * dir, unsigned char *start, unsigned char *end);

/** @}
 */

/** @}
 */

#endif
