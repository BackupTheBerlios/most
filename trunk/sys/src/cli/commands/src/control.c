/*
 * info.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/err.h>
#include <ace/stdlib.h>
#include <cli/arg.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#define CLI_HELP_BUF_SIZE  512

extern ACE_err_t
CLI_cmd_control (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *desc;
    MFS_ctrl_entry_t ctrl;

    int argc;
    char *argv[CLI_MAX_ARG];
    int n = 0;
    
    desc = USO_thread_work_get(USO_current());
    argc = CLI_arg_parse(cli->p.arg, argv);
    if (argc >= 1)
    {
        n = ACE_atoi(argv[0]);
        if (argc >= 3) {
            if (argv[1][0] == 'l') {
                ctrl.type = MFS_CTRL_LONG;
                ctrl.value.l = ACE_strtol(argv[2], NULL, 10);
            } else if (argv[1][0] == 'z') {
                ctrl.type = MFS_CTRL_SIZE;
                ctrl.value.z = (ACE_size_t)ACE_strtoul(argv[2], NULL, 10);
            } else if (argv[1][0] == 'h') {
                ctrl.type = MFS_CTRL_HEX;
                ctrl.value.u = ACE_strtoul(argv[2], NULL, 16);
            } else if (argv[1][0] == 's') {
                ctrl.type = MFS_CTRL_STRING;
                ctrl.value.s = argv[2];
            }
        }
        MFS_control_desc (desc, n, &ctrl);
    }
    else
    {
        char *helpbuf = ACE_malloc(CLI_HELP_BUF_SIZE);
        if (helpbuf) {
            ctrl.type = MFS_CTRL_HELP;
            ctrl.value.s = helpbuf;
            MFS_control_desc (desc, n, &ctrl);
            ACE_printf("%s {\n%s}\n", desc->name, ctrl.value.s);
            ACE_free(helpbuf);
        } else {
            err = DEF_ERR_MEM;
        }
    }
    return err;
}

