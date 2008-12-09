#include <ace/stdlib.h>

#include "mfs/super.h"
#include "mfs/descriptor.h"


extern void
MFS_super_init(MFS_super_t *super,
							struct MFS_vfs_op *vfs_op, void (*info)(void))
{
	MFS_directory_init(&super->dir, vfs_op);
	super->dir.info = info;
	super->operations = vfs_op->super_op;
}


extern MFS_descriptor_t *
MFS_mount (char *name, struct MFS_vfs_op * vfs_op, void (*info)(void))
{
    MFS_descriptor_t *desc = NULL;
	MFS_super_t *super = ACE_malloc( sizeof(MFS_super_t));
	if (super != NULL){
        MFS_super_init(super, vfs_op, info);
	    if (super->operations->mount != NULL) {super->operations->mount(super);}
	    desc = MFS_descriptor_new((MFS_entry_t*)super,
									 &MFS_dir_descriptor_op, name, MFS_SUPER, NULL);
	    if (desc == NULL){
            ACE_free(super);                   
        }
    }
    return desc;
}

extern void
MFS_umount_desc (MFS_descriptor_t *desc)
{
	if (desc->type == MFS_SUPER){
		MFS_super_t *super = (MFS_super_t*)desc->entry;
		if (super->operations->umount != NULL) {super->operations->umount(super);}
		ACE_free(super);
		ACE_free(desc);
	}
}
