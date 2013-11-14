/*
 * goto.c
 *
 *  Created on: 31.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <cli/command.h>
#include <cli/arg.h>
#include <cli/pipe.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_create (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
   int argc;
    char *argv[CLI_MAX_ARG];
    argc = CLI_arg_parse(cli->p.arg, argv);

    MFS_descriptor_t *current = USO_thread_work_get(USO_current());
    if (argc >= 2) {
        switch (argv[0][0]){
            case 'd':
                MFS_directory_create (current, argv[1]);
                break;
            case 'p':
                if (argc >= 3) {
                    CLI_pipe_new ( ACE_atol(argv[2]), current, argv[1]);
                } else {
                    err = DEF_ERR_ARG;
                }
                break;
            case 'f':
                MFS_directory_create_file (current, argv[1]);
                break;
            default:
                err = DEF_ERR_ARG;
                break;
                
        }
    } else {
        err = DEF_ERR_ARG;
    }

    return err;
}


