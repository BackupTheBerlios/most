/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_BSP_COMMANDS_H
#define MDC_BSP_COMMANDS_H

#include <mfs/sysfs.h>

/** @addtogroup init
 *
 * @{
 */

/** @defgroup bsp_commands bsp_commands.h
 *
 * Board support package commandos
 *
 * @{
 */

/**
 * Do a bootp request.
 */
extern void MDC_bootp (void);

/**
 * Start application.
 */
extern void MDC_start_app (void);

/**
 * Start bootloader.
 */
extern void MDC_start_boot (void);

/**
 * Install bsp commands.
 */
extern void MDC_bsp_commands_install (MFS_descriptor_t * bsp);

/** @}
 */

/** @}
 */

#endif
