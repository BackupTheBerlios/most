#include <mfs/descriptor.h>
#include <mfs/super.h>
#include <mfs/directory.h>
#include <mfs/stream.h>

static MFS_descriptor_t *sfs = NULL;

static struct MFS_super_op MFS_sfs_super_op = {
	.mount = NULL,
    .umount = NULL
};

static struct MFS_directory_op MFS_sfs_directory_op = {
    .create = NULL,
    .remove = NULL,
    .rename = NULL
};

static struct MFS_stream_op MFS_sfs_stream_op = {
    .read = NULL,
    .write = NULL,
    .seek = NULL,
    .flush = NULL
};

static struct MFS_vfs_op MFS_sfs_vfs_op = {
	.super_op = &MFS_sfs_super_op,
    .dir_op = &MFS_sfs_directory_op,
    .stream_op = &MFS_sfs_stream_op
};


static void
info (void)
{
    ACE_puts ("small file system\n");
}

extern ACE_bool_t
MFS_sfs_init (void)
{
    sfs = MFS_mount ("sfs", &MFS_sfs_vfs_op, info);
    if (sfs == NULL)
        return FALSE;
    return TRUE;
}

extern MFS_descriptor_t *
MFS_sfs_get_dir (void)
{
    return sfs;
}
