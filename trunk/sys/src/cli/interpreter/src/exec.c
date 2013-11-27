/*
 * exec.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <uso/scheduler.h>
#include <ace/stdio.h>
#include <cli/exec.h>
#include <mfs/vfs.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static MFS_descriptor_t * exe_dir;


static void
exec_info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    CLI_exec_t *exec = (CLI_exec_t *) desc->represent;
    switch (number){
       case 0:
           entry->type = MFS_INFO_STRING;
           entry->name = "Description";
           entry->value.s = exec->description;
           break;
       default:
           entry->type = MFS_INFO_NOT_AVAIL;
           break;
   }
}

static struct MFS_descriptor_op exec_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = exec_info,
    .control = NULL,
    .delete = NULL
};


extern void
CLI_exec_init (MFS_descriptor_t * dir, CLI_exec_t * exec,
               char *name, char *description, ACE_err_t (*f) (char *))
{
    exec->description = description;
    exec->f = f;
    MFS_descriptor_create (dir, name, MFS_EXEC, &exec_descriptor_op, (MFS_represent_t *) exec);
}

extern void
CLI_executes_init (void)
{
    exe_dir = MFS_resolve ("/sys/cli/exe");
}

extern MFS_descriptor_t *
CLI_find_exe (char* name)
{
    MFS_descriptor_t *desc;
    desc = MFS_open (exe_dir, name);
    if (desc == NULL) {
        desc = MFS_open (USO_thread_work_get(USO_current()), name);
    }
    if (NULL != desc && desc->type == MFS_EXEC)
        return desc;
    return NULL;
}
