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


/* Sector 0: putboot, Sector 1: boot, Sector 2-7: app, end: 8 */
#define MDC_FLASH_START  	0
#define MDC_PUTBOOT_SIZE    1
#define MDC_BOOT_SIZE  		1
#define MDC_APPL_SIZE  		6

#define MDC_PUTBOOT_START 	MDC_FLASH_START
#define MDC_PUTBOOT_END 	(MDC_PUTBOOT_START + MDC_PUTBOOT_SIZE)
#define MDC_BOOT_START 		MDC_PUTBOOT_END
#define MDC_BOOT_END   	 	(MDC_BOOT_START + MDC_BOOT_SIZE)
#define MDC_APPL_START    	MDC_BOOT_END
#define MDC_APPL_END      	(MDC_APPL_START + MDC_APPL_SIZE)

/**
 * Install application download commands.
 */
extern void MDC_download_install (MFS_descriptor_t * dir, unsigned int start, unsigned int end);

/** @}
 */

/** @}
 */

#endif
