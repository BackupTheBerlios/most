/*
 * klog.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <cli/command.h>
#include <uso/log.h>

extern ACE_err_t
CLI_cmd_klog (CLI_interpreter_t * cli)
{
    switch (cli->p.arg[0])
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
    return ACE_OK;
}

