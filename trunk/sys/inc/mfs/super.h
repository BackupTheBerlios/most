/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_SUPER_H
#define MFS_SUPER_H

#include "mfs/directory.h"
#include "mfs/vfs.h"

/** @addtogroup mfs
 * @{
 */

/** @defgroup super super.h
 *
 * The root directory with mount and unmount functionality.
 * @{
 */

/* Super structure. */
struct MFS_super
{
    MFS_directory_t dir;        /* It is a directory. */
    struct MFS_super_op *operations;    /* Extended with the mount facility. */
};


/**
 * Initialize the super directory..
 *
 * @param super : Super directory.
 * @param vfs_op : Virtual file system operations.
 * @param info : Callback for printing super information.
 */
extern void MFS_super_init (MFS_super_t * super, struct MFS_vfs_op *vfs_op, void (*info) (void));


/**
 * Mount the super directory..
 *
 * @param name : Directory name.
 * @param vfs_op : Virtual file system operations.
 * @param info : Callback for printing super information.
 * @return Created super descriptor.
 */
extern MFS_descriptor_t *MFS_mount (char *name, struct MFS_vfs_op *vfs_op, void (*info) (void));

/**
 * Unmount super descriptor.
 *
 * @param desc : Super descriptor.
 */
extern void MFS_umount_desc (MFS_descriptor_t * desc);

/** @}
 */

/** @}
 */

#endif
