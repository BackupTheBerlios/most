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

extern ACE_bool_t
CLI_cmd_cd (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = cli->desc;
    if (cli->argc >= 1)
    {
    	if (cli->argv[0][0] == '/')
    	{
    		desc = MFS_get_root();
    	}
    	desc = MFS_resolve (desc, cli->argv[0]);
        if (desc != NULL)
        {
            cli->desc = desc;
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


