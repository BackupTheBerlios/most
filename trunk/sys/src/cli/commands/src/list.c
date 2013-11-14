/*
 * list.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_list (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    ACE_bool_t info = FALSE;
    ACE_bool_t close = FALSE;
    MFS_descriptor_t *desc = NULL;
    if (cli->p.arg[0] != '\0')
    {
        switch (cli->p.arg[0])
        {
        case 'i':
            info = TRUE;
            desc = USO_thread_work_get(USO_current());
            break;
        case 't':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/uso/thread");
            break;
        case 'm':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/uso/heap");
            break;
        case 'c':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/cli/cmd");
            break;
        case 'e':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/cli/exe");
            break;
        case 'a':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/dev/timer/timer");
            break;
        default:
            break;
        }
    } else {
        desc = USO_thread_work_get(USO_current());
    }
    MFS_descriptor_t *iterator = NULL;
    if (desc){
        if ((iterator = MFS_next_entry (desc, iterator)) != NULL) {
            MFS_descriptor_print (iterator, info, TRUE);
            while ((iterator = MFS_next_entry (desc, iterator)) != NULL)
            {
                MFS_descriptor_print (iterator, info, FALSE);
            }
        }
        if (close) MFS_close_desc(desc);
    }
    return err;
}

