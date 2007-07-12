/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_DIRECTORY_H
#define MFS_DIRECTORY_H

#include "uso/list.h"
#include "mfs/vfs.h"
#include "mfs/descriptor.h"
#include "mfs/stream.h"

extern struct MFS_descriptor_op MFS_dir_descriptor_op;

struct MFS_directory
{
	struct MFS_vfs_op *vfs_op;
	USO_list_t descriptors;
	struct MFS_directory_op *operations;
	void (*info)(void);
};

extern MFS_descriptor_t *MFS_next_entry(MFS_descriptor_t *dir_desc, MFS_descriptor_t *iterator);
extern MFS_descriptor_t *MFS_lookup (MFS_descriptor_t *dir_desc, char* name);

extern void MFS_create_desc(MFS_descriptor_t *dir_desc, MFS_descriptor_t *desc);
extern MFS_descriptor_t * MFS_create_dir(MFS_descriptor_t *dir_desc, char *name);
extern MFS_descriptor_t * MFS_create_file(MFS_descriptor_t *dir_desc, char *name);
extern MFS_descriptor_t * MFS_create_io(MFS_descriptor_t *dir_desc, char *name,
				 struct MFS_stream_op *io_op, MFS_stream_represent_t *represent);
extern MFS_descriptor_t * MFS_create_unknowen(MFS_descriptor_t *dir_desc, char *name, MFS_entry_t* entry,
 						enum MFS_entry_type type, struct MFS_descriptor_op *desc_op);

extern void MFS_remove_desc(MFS_descriptor_t *dir_desc, MFS_descriptor_t *desc);
extern void MFS_remove(MFS_descriptor_t *dir_desc, char *name);

extern void MFS_rename_desc(MFS_descriptor_t *dir_desc, MFS_descriptor_t *desc, char *name);
extern MFS_descriptor_t * MFS_rename(MFS_descriptor_t *dir_desc, char *old_name, char *new_name);

extern void MFS_directory_init(MFS_directory_t *dir,
								struct MFS_vfs_op *vfs_op);

#endif
