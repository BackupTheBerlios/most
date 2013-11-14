/*
 * info.c
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
CLI_cmd_info (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *desc;
    if (cli->p.arg[0] != '\0')
    {
        desc = MFS_resolve (cli->p.arg);
        if (desc != NULL)
        {
            MFS_print_info (desc);
            MFS_close_desc (desc);
        }
        else
        {
            err = DEF_ERR_ARG;
        }
    }
    else
    {
        MFS_print_info (USO_thread_work_get(USO_current()));
    }
    return err;
}

