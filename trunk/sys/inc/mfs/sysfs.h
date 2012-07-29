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

/** @addtogroup mfs
 * @{
 */

/** @defgroup sysfs sysfs.h
 *
 * System file system.
 * This is not a real file system, it gives information about the system.
 * Also the executables can be registered.
 * @{
 */

enum MFS_sysfs_dir
{
    MFS_SYSFS_DIR_ROOT,
    MFS_SYSFS_DIR_SYS,
    MFS_SYSFS_DIR_USR,
    MFS_SYSFS_DIR_USO,
    MFS_SYSFS_DIR_THREADS,
    MFS_SYSFS_DIR_HEAPS,
    MFS_SYSFS_DIR_DEV,
    MFS_SYSFS_DIR_SERIAL,
    MFS_SYSFS_DIR_TIMER,
    MFS_SYSFS_DIR_DIGIO,
    MFS_SYSFS_DIR_MM,
    MFS_SYSFS_DIR_NET,
    MFS_SYSFS_DIR_NETIF,
    MFS_SYSFS_DIR_CLI,
    MFS_SYSFS_DIR_CMD
};

/**
 * Initialize system file system.
 * @return 0 if initialization fails.
 */
extern ACE_bool_t MFS_sysfs_init (void);

/**
 * @return Directory descriptor.
 */
extern MFS_descriptor_t *MFS_sysfs_get_dir (enum MFS_sysfs_dir dir);


/** @}
 */

/** @}
 */

#endif
