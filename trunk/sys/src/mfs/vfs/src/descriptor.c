#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>

#include "mfs/descriptor.h"
#include "mfs/directory.h"

extern void
MFS_open_desc (MFS_descriptor_t * desc)
{
    if (desc->operations->open != NULL)
    {
        desc->operations->open (desc->entry);
    }
}

extern MFS_descriptor_t *
MFS_open (MFS_descriptor_t * dir_desc, char *name)
{
    MFS_descriptor_t *desc = NULL;
    if ((dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER))
    {
        desc = MFS_lookup (dir_desc, name);
        if (desc != NULL)
        {
            MFS_open_desc (desc);
        }
    }
    return desc;
}

extern MFS_descriptor_t *
MFS_close_desc (MFS_descriptor_t * desc)
{
    if (desc->operations->close != NULL)
    {
        desc->operations->close (desc->entry);
    }
    return desc->parent;
}

extern void
MFS_info_desc (MFS_descriptor_t * desc)
{
    if (desc->operations->info != NULL)
    {
        desc->operations->info (desc->entry);
    }
}

extern void
MFS_info (MFS_descriptor_t * dir_desc, char *name)
{
    if ((dir_desc->type == MFS_DIRECTORY) || (dir_desc->type == MFS_SUPER))
    {
        MFS_descriptor_t *desc = MFS_lookup (dir_desc, name);
        if (desc != NULL)
        {
            MFS_info_desc (desc);
        }
    }
}

extern void
MFS_descriptor_info (MFS_descriptor_t * desc)
{
    char *type;
    switch (desc->type)
    {
    case MFS_SUPER:
        type = "super";
        break;
    case MFS_DIRECTORY:
        type = "dir";
        break;
    case MFS_STREAM:
        type = "stream";
        break;
    case MFS_EXEC:
        type = "exec";
        break;
    default:
        type = "desc";
        break;
    }
    ACE_printf ("%s \t%s\t", type, desc->name);
}


extern void
MFS_descriptor_init (MFS_descriptor_t * desc,
                     MFS_entry_t * entry,
                     struct MFS_descriptor_op *operations,
                     char *name, enum MFS_entry_type type, MFS_descriptor_t * parent)
{
    desc->entry = entry;
    desc->operations = operations;
    ACE_strncpy (desc->name, name, sizeof (desc->name) - 1);
    desc->name[sizeof (desc->name) - 1] = 0;
    desc->type = type;
    desc->parent = parent;
}

extern MFS_descriptor_t *
MFS_descriptor_new (MFS_entry_t * entry,
                    struct MFS_descriptor_op *operations,
                    char *name, enum MFS_entry_type type, MFS_descriptor_t * parent)
{
    MFS_descriptor_t *desc = ACE_malloc (sizeof (MFS_descriptor_t));
    if (desc != NULL)
    {
        MFS_descriptor_init (desc, entry, operations, name, type, parent);
    }
    return desc;
}
