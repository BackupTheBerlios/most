#include <ace/stdlib.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/super.h>
#include <mfs/directory.h>
#include <mfs/stream.h>

static MFS_descriptor_t *root = NULL;


static void
sysfs_dir_info(MFS_descriptor_t * dir, int number, MFS_info_entry_t *entry)
{
    MFS_directory_info ((MFS_directory_t *)dir, number, entry);
}

static ACE_err_t
sysfs_dir_delete (MFS_descriptor_t *dir)
{
    ACE_err_t err = ACE_OK;
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        if (USO_isempty(&d->descriptors) == FALSE){
            err = DEF_ERR_IN_USE;
        }
    } else {
        err = DEF_ERR_SYS;
    }
    return err;
}


static void
sysfs_stream_info(MFS_descriptor_t * stream, int number, MFS_info_entry_t *entry)
{
    MFS_stream_info ((MFS_stream_t *)stream, number, entry);
}

static ACE_err_t
sysfs_stream_delete (MFS_descriptor_t *stream)
{
    ACE_err_t err = DEF_ERR_SYS;
    if (stream && (stream->type == MFS_STREAM))
    {
        err = ACE_OK;
    }
    return err;
    
}


static struct MFS_descriptor_op MFS_sysfs_super_desc_op = {
    .open = NULL,
    .close = NULL,
    .info = NULL,
    .control = NULL,
    .delete = NULL
};

static struct MFS_super_op MFS_sysfs_super_op = {
    .mount = NULL,
    .umount = NULL
};

static struct MFS_descriptor_op MFS_sysfs_directory_desc_op = {
    .open = NULL,
    .close = NULL,
    .info = sysfs_dir_info,
    .control = NULL,
    .delete = sysfs_dir_delete
};

static struct MFS_directory_op MFS_sysfs_directory_op = {
    .create = NULL,
    .remove = NULL,
    .rename = NULL
};

static struct MFS_descriptor_op MFS_sysfs_stream_desc_op = {
    .open = NULL,
    .close = NULL,
    .info = sysfs_stream_info,
    .control = NULL,
    .delete = sysfs_stream_delete
};

static struct MFS_stream_op MFS_sysfs_stream_op = {
    .read = NULL,
    .write = NULL,
    .seek = NULL,
    .flush = NULL
};

static struct MFS_vfs_op MFS_sysfs_vfs_op = {
    .super_desc_op = &MFS_sysfs_super_desc_op,
    .super_op = &MFS_sysfs_super_op,
    .dir_desc_op = &MFS_sysfs_directory_desc_op,
    .dir_op = &MFS_sysfs_directory_op,
    .stream_desc_op = &MFS_sysfs_stream_desc_op,
    .stream_op = &MFS_sysfs_stream_op
};



extern ACE_bool_t
MFS_sysfs_init (void)
{
    MFS_descriptor_t *sys, *dir, *sub;

    if ( (root = MFS_directory_create_root ("root", &MFS_sysfs_vfs_op)) == NULL) return FALSE;

    if ( (sys = MFS_directory_create (root, "sys")) == NULL) return FALSE;
    if ( (dir = MFS_directory_create (root, "usr")) == NULL) return FALSE;
    if ( (dir = MFS_directory_create (root, "mnt")) == NULL) return FALSE;
    if ( (dir = MFS_directory_create (root, "bsp")) == NULL) return FALSE;
    if ( (dir = MFS_directory_create (root, "app")) == NULL) return FALSE;

    if ( (dir = MFS_directory_create (sys, "uso")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "thread")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "heap")) == NULL) return FALSE;

    if ( (dir = MFS_directory_create (sys, "dev")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "serial")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "timer")) == NULL) return FALSE;
    if ( MFS_directory_create (sub, "timer") == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "mm")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "control")) == NULL) return FALSE;
    if ( MFS_directory_create (sub, "in") == NULL) return FALSE;
    if ( MFS_directory_create (sub, "out") == NULL) return FALSE;

    if ( (dir = MFS_directory_create (sys, "net")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "netif")) == NULL) return FALSE;

    if ( (dir = MFS_directory_create (sys, "cli")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "cmd")) == NULL) return FALSE;
    if ( (sub = MFS_directory_create (dir, "exe")) == NULL) return FALSE;
    return TRUE;
}

extern MFS_descriptor_t *
MFS_get_root (void)
{
    return root;
}
