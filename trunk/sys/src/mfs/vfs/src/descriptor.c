#include <uso/scheduler.h>
#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/assert.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

extern ACE_err_t
MFS_open_desc (MFS_descriptor_t * desc)
{
    ACE_err_t err = ACE_OK;
    if (desc == NULL) return DEF_ERR_NULL;
    USO_lock(&desc->lock);
    /* the open function of the resource is called each time.
     * So the resource must check the open counter and
     * do initialization depending of the open counter!
     * If it is opened more then once it also can return an error
     * to refuse the open request.
     */
    if ( desc->operations->open != NULL )
    {
        err = desc->operations->open (desc);
    }
    if (err == ACE_OK){
        desc->open_cnt++; 
    }
    USO_unlock(&desc->lock);
    return err;
}

extern MFS_descriptor_t *
MFS_open (MFS_descriptor_t * dir, char *name)
{
    MFS_descriptor_t *desc = NULL;
    desc = MFS_lookup (dir, name);
    if (desc != NULL)
    {
        ACE_err_t err = MFS_open_desc (desc);
        if ( err != ACE_OK) {
            desc = NULL;
        }
    }
    return desc;
}

extern MFS_descriptor_t *
MFS_close_desc (MFS_descriptor_t * desc)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *parent = NULL;
    if (desc){
        USO_lock(&desc->lock);
        if (desc->open_cnt > 0){ 
            /* the close function of the resource is called each time.
             * So the resource must check the close counter and
             * do cleanup depending of the close counter!
             */
            if (desc->operations->close != NULL)
            {
                err = desc->operations->close (desc);
            }
            if (err == ACE_OK){
                desc->open_cnt--;
                parent = desc->parent;
            }
        }
        USO_unlock(&desc->lock);
    }
    return parent;
}

extern MFS_descriptor_t *
MFS_walk_in (MFS_descriptor_t * dir, char *name, ACE_bool_t close)
{
    MFS_descriptor_t *desc = NULL;
    desc = MFS_open(dir, name);
    if (close) MFS_close_desc(dir);
    return desc;
}

extern MFS_descriptor_t *
MFS_walk_out (MFS_descriptor_t * desc, ACE_bool_t close)
{
    MFS_descriptor_t *parent = NULL;
    if (desc){
        parent = desc->parent;
        if (close) MFS_close_desc(desc);
        if (parent){ 
            MFS_open_desc(parent);
        }
    }
    return parent;
}

extern MFS_descriptor_t *
MFS_resolve (char* s)
{
    MFS_descriptor_t *next, *curr;
    char path[MFS_PATH_SIZE]; /* s can be placed in flash rom, so copy it to ram because ACE_strtok is writing to it*/

    ACE_SANITY_CHECK(s);

    if (*s == '/'){
        curr = MFS_get_root();
    } else {
        curr = USO_thread_work_get (USO_current());
    }
    if (curr == NULL) return NULL;
    next = curr;

    USO_lock (&ACE_lock);

    ACE_strncpy(path, s, MFS_PATH_SIZE - 1);
    path[MFS_PATH_SIZE-1] = '\0';
    for (char* name = ACE_strtok(path, "/"); name != NULL; name = ACE_strtok(NULL, "/")){
        if (ACE_strcmp ( "..", name) == 0){
            next = MFS_walk_out(next, next == curr ? FALSE : TRUE);
        } else if (ACE_strcmp ( ".", name) == 0){
            ;
        } else {
            next = MFS_walk_in (next, name, next == curr ? FALSE : TRUE);
        }
        if (next == NULL) {
            break;
        }
    }
    USO_unlock (&ACE_lock);

    return next;
}

extern void
MFS_info_desc (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    ACE_SANITY_CHECK(desc);
    if (desc->operations->info != NULL)
    {
        desc->operations->info (desc, number, entry);
    }
    else
    {
        entry->type = MFS_INFO_NOT_AVAIL;
    }
}


extern void
MFS_control_desc (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    ACE_SANITY_CHECK(desc);
    if (desc->operations->control != NULL)
    {
        desc->operations->control (desc, number, entry);
    }
    else
    {
        entry->type = MFS_CTRL_INFO;
        entry->value.s = "ctrl not avail";
    }
}




extern void
MFS_descriptor_init (MFS_descriptor_t * desc,
                MFS_represent_t *represent,
                struct MFS_descriptor_op *operations,
                char *name, enum MFS_desc_type type, MFS_descriptor_t * parent)
{
    ACE_SANITY_CHECK(desc);
    desc->represent = represent;
    desc->operations = operations;
    ACE_strncpy (desc->name, name, sizeof (desc->name) - 1);
    desc->name[sizeof (desc->name) - 1] = 0;
    desc->type = type;
    desc->parent = parent;
    desc->open_cnt = 0;
    USO_mutex_init(&desc->lock);
}


extern MFS_descriptor_t *
MFS_descriptor_create (MFS_descriptor_t * dir, char *name, enum MFS_desc_type type,
                 struct MFS_descriptor_op *desc_op, MFS_represent_t * represent)
{
    MFS_descriptor_t *desc = NULL;
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        desc = ACE_malloc (sizeof (MFS_descriptor_t));
        if (desc != NULL)
        {
            MFS_descriptor_init(desc, represent, desc_op, name, type, dir);
            MFS_create_desc (dir, desc);
        }
    }
    return desc;
}



