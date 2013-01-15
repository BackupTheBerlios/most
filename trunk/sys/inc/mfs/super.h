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
 * @{
 */

/* Super structure. */
struct MFS_super
{
    struct MFS_super_op *operations;    /* ?. */
};


extern void MFS_super_init (MFS_super_t * super, struct MFS_vfs_op *vfs_op);

extern void MFS_mount (MFS_super_t * super);

extern void MFS_umount (MFS_super_t * super);

/** @}
 */

/** @}
 */

#endif
