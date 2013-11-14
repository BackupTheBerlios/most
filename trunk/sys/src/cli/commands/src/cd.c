/*
 * goto.c
 *
 *  Created on: 31.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_cd (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK; 
    MFS_descriptor_t *desc, *curr;
    curr = USO_thread_work_get (USO_current());
    desc = MFS_resolve (cli->p.arg);
    if (desc != NULL)
    {
        USO_thread_work_set (USO_current(), desc);
        MFS_close_desc(curr);
    }
    else
    {
        err = DEF_ERR_ARG;
    }
    return err;
}


