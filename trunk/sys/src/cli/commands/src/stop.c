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

extern ACE_bool_t
CLI_cmd_stop (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = MFS_resolve(MFS_get_root(), "sys/uso/thread");
    if (cli->argc >= 1)
    {
        desc = MFS_lookup (desc, cli->argv[0]);
        if (desc != NULL && desc->type == MFS_SYS)
        {
            USO_stop ((USO_thread_t *) desc->represent);
            done = TRUE;
        }
        else
        {
            ACE_puts (CLI_err_arg_notfound);
        }
    }
    else
    {
        ACE_puts (CLI_err_arg_missing);
    }
    return done;
}

