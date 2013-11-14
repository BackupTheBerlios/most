#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/assert.h>
#include <uso/list.h>
#include <mfs/err.h>
#include <mfs/directory.h>
#include <mfs/stream.h>




extern MFS_descriptor_t *
MFS_next_entry (MFS_descriptor_t * dir, MFS_descriptor_t * iterator)
{
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        iterator = (MFS_descriptor_t *) USO_next_element (&d->descriptors,
                        (USO_node_t *) iterator);
    } else {
        iterator = NULL;
    }
    return iterator;
}

extern MFS_descriptor_t *
MFS_lookup (MFS_descriptor_t * dir, char *name)
{
    MFS_descriptor_t *desc = NULL;
    ACE_SANITY_CHECK(name);
    if ( dir && (dir->type == MFS_DIRECTORY))
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        while ((desc = (MFS_descriptor_t *) USO_next_element (&d->descriptors,
                                                              (USO_node_t *) desc)) != NULL)
        {
            if (ACE_strncmp (desc->name, name, sizeof (desc->name)) == 0)
            {
                break;
            }
        }
    }
    return desc;
}

extern void
MFS_create_desc (MFS_descriptor_t * dir, MFS_descriptor_t * desc)
{
    ACE_SANITY_CHECK(dir);
    ACE_SANITY_CHECK(desc);
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        if (d->operations->create != NULL)
        {
            d->operations->create (desc);
        }
        USO_push (&d->descriptors, (USO_node_t *) desc);
    }
}

extern ACE_err_t
MFS_remove_desc (MFS_descriptor_t * dir, MFS_descriptor_t * desc)
{
    ACE_err_t err = ACE_OK;
    ACE_SANITY_CHECK(dir);
    ACE_SANITY_CHECK(desc);
    if (dir && dir->type == MFS_DIRECTORY)
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        if (desc->operations->delete != NULL){
            err = desc->operations->delete(desc);
            if (err == ACE_OK){
                if (d->operations->remove != NULL)
                {
                    d->operations->remove (desc);
                }
                USO_remove (&d->descriptors, (USO_node_t *) desc);
                ACE_free (desc);
            }
        }
    }
    return err;
}

extern ACE_err_t
MFS_remove (MFS_descriptor_t * dir, char *name)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *desc = MFS_lookup (dir, name);
    if (desc != NULL)
    {
        err = MFS_remove_desc (dir, desc);
    } else {
        err = DEF_ERR_NOT_FOUND;
    }
    return err;
}

extern void
MFS_rename_desc (MFS_descriptor_t * dir, MFS_descriptor_t * desc, char *name)
{
    ACE_SANITY_CHECK(name);
    ACE_SANITY_CHECK(dir);
    ACE_SANITY_CHECK(desc);
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        if (d->operations->rename != NULL)
        {
            d->operations->rename (desc);
        }
        ACE_strncpy (desc->name, name, sizeof (desc->name) - 1);
    }
}

extern MFS_descriptor_t *
MFS_rename (MFS_descriptor_t * dir, char *old_name, char *new_name)
{
    MFS_descriptor_t *desc = NULL;
    desc = MFS_lookup (dir, old_name);
    if (desc != NULL)
    {
        MFS_rename_desc (dir, desc, new_name);
    }
    return desc;
}



extern void
MFS_directory_init (MFS_directory_t * dir, struct MFS_vfs_op *vfs_op)
{
    ACE_SANITY_CHECK(dir);
    USO_list_init (&dir->descriptors);
    dir->vfs_op = vfs_op;
    dir->operations = vfs_op->dir_op;
}

extern MFS_descriptor_t *
MFS_directory_create (MFS_descriptor_t * dir, char *name)
{
    MFS_directory_t *new_dir = NULL;
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        new_dir = ACE_malloc (sizeof (MFS_directory_t));
        if (new_dir != NULL)
        {
            MFS_directory_t *d = (MFS_directory_t *) dir;
            MFS_descriptor_init((MFS_descriptor_t *)new_dir, NULL, d->vfs_op->dir_desc_op, name, MFS_DIRECTORY, dir);
            MFS_directory_init (new_dir, d->vfs_op);
            MFS_create_desc (dir, (MFS_descriptor_t *)new_dir);
        }
    }
    return (MFS_descriptor_t *)new_dir;
}

extern MFS_descriptor_t *
MFS_directory_create_root (char *name, struct MFS_vfs_op *vfs_op)
{
    MFS_directory_t *root = ACE_malloc (sizeof (MFS_directory_t));

    if (root != NULL)
    {
        MFS_descriptor_init ( (MFS_descriptor_t *) root, NULL, vfs_op->dir_desc_op, name, MFS_DIRECTORY, NULL);
        MFS_directory_init (root, vfs_op);
    }
    return (MFS_descriptor_t *) root;
}

extern MFS_descriptor_t *
MFS_directory_create_file (MFS_descriptor_t * dir, char *name)
{
    MFS_descriptor_t *file = NULL;
    if (dir && dir->type == MFS_DIRECTORY)
    {
        MFS_directory_t *d = (MFS_directory_t *) dir;
        file = MFS_stream_create (dir, name, d->vfs_op->stream_desc_op, d->vfs_op->stream_op, NULL, MFS_FILE);
    }
    return file;
}

extern void
MFS_directory_info (MFS_directory_t * dir, int number, MFS_info_entry_t *entry)
{
    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "info";
            entry->value.s = "nix";
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}
