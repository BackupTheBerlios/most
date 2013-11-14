/*
 * info.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <cli/arg.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_control (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *desc;
    MFS_ctrl_entry_t ctrl;

    int argc;
    char *argv[CLI_MAX_ARG];
    char infobuf[128];
    
    desc = USO_thread_work_get(USO_current());
    argc = CLI_arg_parse(cli->p.arg, argv);
    if (argc >= 1)
    {
        int n = ACE_atoi(argv[0]);
        if (n && argc >= 3) {
            if (argv[1][0] == 'l') {
                ctrl.type = MFS_CTRL_LONG;
                ctrl.value.l = ACE_atol(argv[2]);
            } else if (argv[1][0] == 'z') {
                ctrl.type = MFS_CTRL_SIZE;
                ctrl.value.z = (ACE_size_t)ACE_atol(argv[2]);   /* todo this does not work for negative values */
            } else if (argv[1][0] == 's') {
                ctrl.type = MFS_CTRL_STRING;
                ctrl.value.s = argv[2];
            }
        } else {
            ctrl.type = MFS_CTRL_INFO;
            ctrl.value.s = infobuf;
        }
        MFS_control_desc (desc, n, &ctrl);
        if (n == 0){
            ACE_printf("%s\n", ctrl.value.s);
        }
    }
    else
    {
        err = DEF_ERR_ARG;
    }
    return err;
}

