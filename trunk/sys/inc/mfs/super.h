/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_SUPER_H
#define MFS_SUPER_H

#include "mfs/directory.h"
#include "mfs/vfs.h"

struct MFS_super
{
	MFS_directory_t dir;
	struct MFS_super_op *operations;
};

extern void MFS_super_init(MFS_super_t *super,
				 struct MFS_vfs_op *vfs_op, void (*info)(void));

extern MFS_descriptor_t * MFS_mount (char *name,
				 struct MFS_vfs_op * vfs_op, void (*info)(void));
extern void MFS_umount_desc (MFS_descriptor_t *desc);



#endif
