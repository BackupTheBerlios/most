#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/list.h>

#include "mfs/directory.h"
#include "mfs/descriptor.h"
#include "mfs/super.h"
#include "mfs/stream.h"


static void dir_open(MFS_entry_t *entry)
{
	MFS_directory_t * dir = (MFS_directory_t *)entry;
	if (dir->operations->open != NULL) {dir->operations->open(dir);}
}

static void dir_close(MFS_entry_t *entry)
{
	MFS_directory_t * dir = (MFS_directory_t *)entry;
	if (dir->operations->close != NULL) {dir->operations->close(dir);}
}

static void dir_info(MFS_entry_t *entry)
{
	MFS_directory_t *dir = (MFS_directory_t*)entry;
	if (dir->info != NULL) { dir->info();}
	else { putc('\n');}
}


struct MFS_descriptor_op MFS_dir_descriptor_op = {.open = dir_open,
											 .close = dir_close,
										     .info = dir_info};


extern MFS_descriptor_t *
MFS_next_entry(MFS_descriptor_t *dir_desc, MFS_descriptor_t *iterator)
{
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		iterator = (MFS_descriptor_t*)USO_next_element(&dir->descriptors,
											 (USO_node_t*)iterator);
	}
	return iterator;
}

extern MFS_descriptor_t *
MFS_lookup(MFS_descriptor_t *dir_desc, char* name)
{
	MFS_descriptor_t *desc = NULL;
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		while ( (desc = (MFS_descriptor_t*)USO_next_element(&dir->descriptors,
												 (USO_node_t*)desc)) != NULL)
		{
			if ( strncmp(desc->name, name, sizeof(desc->name)) == 0){ 
				break;
			}
		}
	}
	return desc;
}

static void
create_desc(MFS_descriptor_t *dir_desc, MFS_descriptor_t *desc)
{
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		if (dir->operations->create != NULL) {dir->operations->create(desc);}
		USO_push(&dir->descriptors, (USO_node_t*)desc);
	}
}

extern MFS_descriptor_t *
MFS_create_dir(MFS_descriptor_t *dir_desc, char *name)
{
	MFS_descriptor_t *desc = NULL;
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		MFS_directory_t *new_dir = malloc( sizeof(MFS_directory_t));
		if (new_dir != NULL) {
            MFS_directory_init(new_dir, dir->vfs_op);
		    desc = MFS_descriptor_new((MFS_entry_t*)new_dir, &MFS_dir_descriptor_op,
								 name, MFS_DIRECTORY, dir_desc);
		    if (desc != NULL){
                create_desc(dir_desc, desc);
            } else {
                free(new_dir);
            }
        }
	}
	return desc;
}

extern MFS_descriptor_t *
MFS_create_file(MFS_descriptor_t *dir_desc, char *name)
{
	MFS_descriptor_t *desc = NULL;
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		MFS_stream_t *file = malloc( sizeof(MFS_stream_t));
		if (file != NULL){
            MFS_stream_init(file, MFS_FILE, dir->vfs_op->stream_op, NULL);
		    desc = MFS_descriptor_new((MFS_entry_t*)file, &MFS_stream_descriptor_op,
									 name, MFS_STREAM, dir_desc);
		    if (desc != NULL){
                create_desc(dir_desc, desc);
            } else {
                free(file);            
            }
        }
	}
	return desc;
}

extern MFS_descriptor_t *
MFS_create_io(MFS_descriptor_t *dir_desc, char *name, struct MFS_stream_op *io_op,
		 MFS_stream_represent_t *represent)
{
	MFS_descriptor_t *desc = NULL;
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_stream_t *io = malloc( sizeof(MFS_stream_t));
		if (io != NULL) {
            MFS_stream_init(io, MFS_IO, io_op, represent);
		    desc = MFS_descriptor_new((MFS_entry_t*)io, &MFS_stream_descriptor_op,
									 name, MFS_STREAM, dir_desc);
		    if (desc != NULL) {  
                create_desc(dir_desc, desc);
            } else {
                free(io);
            }
        }
	}
	return desc;
}

extern MFS_descriptor_t *
MFS_create_desc(MFS_descriptor_t *dir_desc, char *name, MFS_entry_t* entry,
					enum MFS_entry_type type, struct MFS_descriptor_op *desc_op)
{
	MFS_descriptor_t *desc = NULL;
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		desc = MFS_descriptor_new(entry, desc_op, name, type, dir_desc);
		if (desc != NULL) {
            create_desc(dir_desc, desc);
        }
	}
	return desc;
}



extern void
MFS_remove_desc(MFS_descriptor_t *dir_desc, MFS_descriptor_t *desc)
{
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		if (dir->operations->remove != NULL) {dir->operations->remove(desc);}
		USO_remove(&dir->descriptors, (USO_node_t*)desc);
		if ( (desc->type == MFS_STREAM) || (desc->type == MFS_DIRECTORY) ) {
			free(desc->entry);
			free(desc);
		} else if (desc->type != MFS_SUPER){
			free(desc);
		}
	}
}

extern void
MFS_remove(MFS_descriptor_t *dir_desc, char *name)
{
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_descriptor_t *desc = MFS_lookup(dir_desc, name);
		if (desc != NULL){
			MFS_remove_desc(dir_desc, desc);
		}
	}
}

extern void
MFS_rename_desc(MFS_descriptor_t *dir_desc, MFS_descriptor_t *desc, char *name)
{
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		MFS_directory_t *dir = (MFS_directory_t *)dir_desc->entry;
		//MFS_desc_rename(desc, name)
		if (dir->operations->rename != NULL) {dir->operations->rename(desc);}
	}
}

extern 	MFS_descriptor_t *
MFS_rename(MFS_descriptor_t *dir_desc, char *old_name, char *new_name)
{
	MFS_descriptor_t *desc = NULL;
	if ( (dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER) ){
		desc = MFS_lookup(dir_desc, old_name);
		if (desc != NULL){
			MFS_rename_desc(dir_desc, desc, new_name);
		}
	}
	return desc;
}



extern void
MFS_directory_init(MFS_directory_t *dir,
						struct MFS_vfs_op *vfs_op)
{
	USO_list_init(&dir->descriptors);
	dir->vfs_op = vfs_op;
	dir->operations = vfs_op->dir_op;
	dir->info = NULL;
}
