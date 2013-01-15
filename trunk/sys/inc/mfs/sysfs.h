/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_SYSFS_H
#define MFS_SYSFS_H

#include <ace/stddef.h>
#include <mfs/descriptor.h>

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

/**
 * Initialize system file system.
 * @return 0 if initialization fails.
 */
extern ACE_bool_t MFS_sysfs_init (void);

/**
 * @return Root Directory.
 */
extern MFS_descriptor_t *MFS_get_root (void);


/** @}
 */

/** @}
 */

#endif
