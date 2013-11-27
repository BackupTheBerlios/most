/*
 * klog.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <cli/command.h>
#include <cli/arg.h>
#include <uso/log.h>

extern ACE_err_t
CLI_cmd_klog (CLI_interpreter_t * cli)
{
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(cli->p.arg, argv);
    if (argc >= 1){
        switch (argv[0][0])
        {
            case '+':
                USO_log_inc ();
                break;
            case '-':
                USO_log_dec ();
                break;
            default:
                USO_log_show ();
                break;
        }
    } else {
        USO_log_show ();
    }
    return ACE_OK;
}

