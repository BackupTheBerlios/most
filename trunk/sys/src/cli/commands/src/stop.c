/*
 * stop.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <uso/thread.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_stop (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *dir, *desc;
    dir = MFS_resolve("/sys/uso/thread");
    desc = MFS_lookup (dir, cli->p.arg);
    MFS_close_desc(dir);
    if (desc != NULL && desc->type == MFS_SYS)
    {
        USO_stop ((USO_thread_t *) desc->represent);
    }
    else
    {
        err = DEF_ERR_ARG;
    }
    return err;
}

