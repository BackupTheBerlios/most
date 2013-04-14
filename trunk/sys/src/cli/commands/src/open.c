/*
 * open.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>

extern ACE_bool_t
CLI_cmd_open (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = USO_thread_dir_get(USO_current());
    if (cli->argc >= 1)
    {
        desc = MFS_open (desc, cli->argv[0]);
        if (desc != NULL)
        {
            USO_thread_dir_set (USO_current(), desc);
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


