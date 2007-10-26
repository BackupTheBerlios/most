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

extern bool_t MFS_sysfs_init(void);
extern MFS_descriptor_t *MFS_sysfs_root(void);
extern MFS_descriptor_t *MFS_sysfs_serial(void);
extern MFS_descriptor_t *MFS_sysfs_threads(void);
extern MFS_descriptor_t *MFS_sysfs_heaps(void);
extern MFS_descriptor_t *MFS_sysfs_net(void);
extern MFS_descriptor_t *MFS_sysfs_netif(void);
extern MFS_descriptor_t *MFS_sysfs_cli(void);

#endif
