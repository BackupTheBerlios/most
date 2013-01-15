/*
 * close.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>

extern ACE_bool_t
CLI_cmd_close (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc;
    desc = MFS_close_desc (cli->desc);
    if (desc != NULL)
    {
        cli->desc = desc;
        done = TRUE;
    }
    else
    {
        ACE_puts (CLI_err_sys_fail);
    }
    return done;
}

