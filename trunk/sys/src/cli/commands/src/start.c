/*
 * start.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <uso/thread.h>
#include <cli/command.h>
#include <cli/arg.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_start (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *dir, *desc;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(cli->p.arg, argv);
    if (argc >= 1){
        dir = MFS_resolve("/sys/uso/thread");
        desc = MFS_lookup (dir, argv[0]);
        MFS_close_desc(dir);
        if (desc != NULL && desc->type == MFS_SYS)
        {
            USO_start ((USO_thread_t *) desc->represent);
        }
        else
        {
            err = DEF_ERR_NOT_FOUND;
        }
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}


