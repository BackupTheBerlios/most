/*
 * goto.c
 *
 *  Created on: 31.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <cli/arg.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_rename (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    int argc;
    char *argv[CLI_MAX_ARG];
    argc = CLI_arg_parse(cli->p.arg, argv);

    MFS_descriptor_t *dir = USO_thread_work_get(USO_current());
    if (argc >= 2) {
        MFS_rename (dir, argv[0], argv[1]);
    } else {
        err = DEF_ERR_ARG;
    }

    return err;
}


