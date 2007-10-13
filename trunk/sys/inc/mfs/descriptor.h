/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_DESCRIPTOR_H
#define MFS_DESCRIPTOR_H

#define MFS_NAME_SIZE 8

#include "uso/list.h"
#include "mfs/vfs.h"

enum MFS_entry_type
{
	MFS_SUPER,
	MFS_DIRECTORY,
	MFS_STREAM,
	MFS_EXEC,
	MFS_DESC
};

struct MFS_descriptor
{
	USO_node_t node;
	MFS_entry_t *entry;
	struct MFS_descriptor_op *operations;
	char name[MFS_NAME_SIZE];
	enum MFS_entry_type type;
	MFS_descriptor_t *parent;
}; 

extern void MFS_open_desc(MFS_descriptor_t *desc);
extern MFS_descriptor_t * MFS_open(MFS_descriptor_t *dir_desc, char*name);
extern MFS_descriptor_t * MFS_close_desc(MFS_descriptor_t *desc);
extern void MFS_info_desc(MFS_descriptor_t *desc);
extern void MFS_info(MFS_descriptor_t *dir_desc, char*name);
	
extern void MFS_descriptor_info(MFS_descriptor_t *desc);

extern void MFS_descriptor_init(MFS_descriptor_t *desc,
							    MFS_entry_t *entry,		
								struct MFS_descriptor_op *operations,
								char *name,
								enum MFS_entry_type type,
								MFS_descriptor_t *parent);

extern MFS_descriptor_t *MFS_descriptor_new(MFS_entry_t *entry,		
								struct MFS_descriptor_op *operations,
								char *name,
								enum MFS_entry_type type,
								MFS_descriptor_t *parent);
								
#endif
