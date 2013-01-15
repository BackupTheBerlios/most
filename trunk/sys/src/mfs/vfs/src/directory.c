#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/list.h>
#include <mfs/err.h>
#include <mfs/directory.h>




extern MFS_descriptor_t *
MFS_next_entry (MFS_descriptor_t * dir_desc, MFS_descriptor_t * iterator)
{
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
        iterator = (MFS_descriptor_t *) USO_next_element (&dir->descriptors,
                                                          (USO_node_t *) iterator);
    }
    return iterator;
}

extern MFS_descriptor_t *
MFS_lookup (MFS_descriptor_t * dir_desc, char *name)
{
    MFS_descriptor_t *desc = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
        while ((desc = (MFS_descriptor_t *) USO_next_element (&dir->descriptors,
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
MFS_create_desc (MFS_descriptor_t * dir_desc, MFS_descriptor_t * desc)
{
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
        if (dir->operations->create != NULL)
        {
            dir->operations->create (desc);
        }
        USO_push (&dir->descriptors, (USO_node_t *) desc);
    }
}

extern void
MFS_remove_desc (MFS_descriptor_t * dir_desc, MFS_descriptor_t * desc)
{
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
        USO_remove (&dir->descriptors, (USO_node_t *) desc);
        if (dir->operations->remove != NULL)
        {
            dir->operations->remove (desc);
        }
        ACE_free (desc);
    }
}

extern void
MFS_remove (MFS_descriptor_t * dir_desc, char *name)
{
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_descriptor_t *desc = MFS_lookup (dir_desc, name);
        if (desc != NULL)
        {
            MFS_remove_desc (dir_desc, desc);
        }
    }
}

extern void
MFS_rename_desc (MFS_descriptor_t * dir_desc, MFS_descriptor_t * desc, char *name)
{
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
        // todo MFS_desc_rename(desc, name)
        if (dir->operations->rename != NULL)
        {
            dir->operations->rename (desc);
        }
    }
}

extern MFS_descriptor_t *
MFS_rename (MFS_descriptor_t * dir_desc, char *old_name, char *new_name)
{
    MFS_descriptor_t *desc = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        desc = MFS_lookup (dir_desc, old_name);
        if (desc != NULL)
        {
            MFS_rename_desc (dir_desc, desc, new_name);
        }
    }
    return desc;
}



extern void
MFS_directory_init (MFS_directory_t * dir, struct MFS_vfs_op *vfs_op)
{
    USO_list_init (&dir->descriptors);
    dir->vfs_op = vfs_op;
    dir->operations = vfs_op->dir_op;
}

extern MFS_descriptor_t *
MFS_directory_create (MFS_descriptor_t * dir_desc, char *name)
{
	MFS_directory_t *new_dir = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        new_dir = ACE_malloc (sizeof (MFS_directory_t));
        if (new_dir != NULL)
        {
            MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
           	MFS_descriptor_init((MFS_descriptor_t *)new_dir, NULL, dir->vfs_op->dir_desc_op, name, MFS_DIRECTORY, dir_desc);
           	MFS_directory_init (new_dir, dir->vfs_op);
           	MFS_create_desc (dir_desc, (MFS_descriptor_t *)new_dir);
        }
    }
    return (MFS_descriptor_t *)new_dir;
}

extern MFS_descriptor_t *
MFS_directory_create_root (char *name, struct MFS_vfs_op *vfs_op)
{
    MFS_directory_t *root_dir = ACE_malloc (sizeof (MFS_directory_t));
    if (root_dir != NULL)
    {
        MFS_descriptor_init ( (MFS_descriptor_t *) root_dir, NULL, vfs_op->dir_desc_op, name, MFS_DIRECTORY, NULL);
       	MFS_directory_init (root_dir, vfs_op);
    }
    return (MFS_descriptor_t *) root_dir;
}

extern void
MFS_directory_print (MFS_directory_t * dir)
{
    ACE_putc ('\n');
}
