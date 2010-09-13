/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_SYSFS_H
#define MFS_SYSFS_H

#include "ace/stddef.h"
#include "uso/thread.h"
#include "mfs/vfs.h"
#include "cli/commands.h"

/** @defgroup sysfs sysfs.h
 *
 * System file system.
 * This is not a real file system, it gives information about the system.
 * Also the executables can be registered.
 * @{
 */

/**
 * Initialize system file system.
 * @return 0 if initialization fails.
 */
extern ACE_bool_t MFS_sysfs_init(void);

/**
 * @return Root directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_root(void);

/**
 * @return Serial directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_serial(void);

/**
 * @return Threads directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_threads(void);

/**
 * @return Heaps directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_heaps(void);

/**
 * @return NET directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_net(void);

/**
 * @return Netif directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_netif(void);

/**
 * @return CLI directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_cli(void);

/** @}
 */

#endif
