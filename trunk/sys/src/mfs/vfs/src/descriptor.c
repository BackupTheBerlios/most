#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

extern ACE_err_t
MFS_open_desc (MFS_descriptor_t * desc)
{
	desc->open_cnt++; /** todo synchronize desc->open_cnt */
    if (desc->open_cnt == 1 && desc->operations->open != NULL)
    {
        return desc->operations->open (desc);
    }
    return ACE_OK;
}

extern MFS_descriptor_t *
MFS_open (MFS_descriptor_t * dir_desc, char *name)
{
    MFS_descriptor_t *desc = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        desc = MFS_lookup (dir_desc, name);
        if (desc != NULL)
        {
            if (MFS_open_desc (desc) != ACE_OK) return NULL;
        }
    }
    return desc;
}

extern MFS_descriptor_t *
MFS_close_desc (MFS_descriptor_t * desc)
{
    if (desc->open_cnt > 0){ /** todo synchronize desc->open_cnt */
    	desc->open_cnt--;
    	if (desc->open_cnt == 0 && desc->operations->close != NULL)
    	{
    		desc->operations->close (desc);
    	}
    	return desc->parent;
    }
    return NULL;
}

extern MFS_descriptor_t *
MFS_resolve (MFS_descriptor_t* desc, char* s)
{
	MFS_descriptor_t *next = desc;
	char path[MFS_PATH_SIZE]; /* s can be placed in flash rom, so copy it to ram because ACE_strtok is writing to it*/
	ACE_strncpy(path, s, MFS_PATH_SIZE - 1);
	path[MFS_PATH_SIZE-1] = '\0';
	USO_lock (&ACE_lock);
	for (char* name = ACE_strtok(path, "/"); name != NULL; name = ACE_strtok(NULL, "/")){
		if (ACE_strcmp ( "..", name) == 0){
			next = MFS_close_desc(next);
		} else {
			next = MFS_open (next, name);
		}
		if (next == NULL)
			break;
	}
	USO_unlock (&ACE_lock);
	return next;
}

extern void
MFS_info_desc (MFS_descriptor_t * desc)
{
    if (desc->operations->info != NULL)
    {
        desc->operations->info (desc);
    }
}

extern void
MFS_info (MFS_descriptor_t * dir_desc, char *name)
{
    if (dir_desc->type == MFS_DIRECTORY)
    {
        MFS_descriptor_t *desc = MFS_lookup (dir_desc, name);
        if (desc != NULL)
        {
            MFS_info_desc (desc);
        }
    }
}

extern void
MFS_control_desc (MFS_descriptor_t * desc, enum MFS_control_key key, long value)
{
    if (desc->operations->control != NULL)
    {
        desc->operations->control (desc, key, value);
    }
}




extern void
MFS_descriptor_init (MFS_descriptor_t * desc,
					 MFS_represent_t *represent,
                     struct MFS_descriptor_op *operations,
                     char *name, enum MFS_desc_type type, MFS_descriptor_t * parent)
{
	desc->represent = represent;
    desc->operations = operations;
    ACE_strncpy (desc->name, name, sizeof (desc->name) - 1);
    desc->name[sizeof (desc->name) - 1] = 0;
    desc->type = type;
    desc->parent = parent;
    desc->open_cnt = 0;
}


extern MFS_descriptor_t *
MFS_descriptor_create (MFS_descriptor_t * dir_desc, char *name, enum MFS_desc_type type,
                 struct MFS_descriptor_op *desc_op, MFS_represent_t * represent)
{
    MFS_descriptor_t *desc = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        desc = ACE_malloc (sizeof (MFS_descriptor_t));
        if (desc != NULL)
        {
        	MFS_descriptor_init(desc, represent, desc_op, name, type, dir_desc);
            MFS_create_desc (dir_desc, desc);
        }
    }
    return desc;
}


extern void
MFS_descriptor_print (MFS_descriptor_t * desc)
{
    char *type;
    switch (desc->type)
    {
    case MFS_DIRECTORY:
        type = "dir";
        break;
    case MFS_STREAM:
        type = "stream";
        break;
    case MFS_BLOCK:
        type = "block";
        break;
    case MFS_EXEC:
        type = "exec";
        break;
    case MFS_INFO:
        type = "info";
        break;
    case MFS_SYS:
        type = "sys";
        break;
    case MFS_USR:
        type = "usr";
        break;
    default:
        type = "unknowen";
        break;
    }
    ACE_printf ("%s\t%s\t%i\t", type, desc->name, desc->open_cnt);
}

