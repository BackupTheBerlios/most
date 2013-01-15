#include <ace/stdlib.h>

#include "mfs/super.h"
#include "mfs/descriptor.h"


extern void
MFS_super_init (MFS_super_t * super, struct MFS_vfs_op *vfs_op)
{
    super->operations = vfs_op->super_op;
}


extern void
MFS_mount (MFS_super_t * super)
{
	if (super->operations->mount != NULL)
	{
		super->operations->mount (super);
	}
}

extern void
MFS_umount (MFS_super_t * super)
{
	if (super->operations->umount != NULL)
	{
		super->operations->umount (super);
	}
}
