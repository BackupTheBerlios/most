/*
 * close.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>

extern ACE_err_t
CLI_cmd_close (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK; 
    MFS_descriptor_t *desc;
    desc = MFS_close_desc (USO_thread_work_get(USO_current()));
    if (desc != NULL)
    {
        MFS_open_desc(desc);
        USO_thread_work_set (USO_current(), desc);
    }
    else
    {
        err = CLI_ERR_NO_PARENT;
    }
    return err;
}

