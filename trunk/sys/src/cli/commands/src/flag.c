/*
 * goto.c
 *
 *  Created on: 31.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <uso/thread.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/sysfs.h>

extern ACE_err_t
CLI_cmd_flag (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    if (cli->p.arg[0] != '\0')
    {
        int prio = USO_USER;
        int sched = USO_ROUND_ROBIN;
        switch (cli->p.arg[0])
        {
        case 'u':
            prio = USO_USER;
            break;
        case 's':
            prio = USO_SYSTEM;
            break;
        case 'i':
            prio = USO_INTERRUPT;
            break;
        default:
            break;
        }
        switch (cli->p.arg[1])
        {
        case 'f':
            sched = USO_FIFO;
            break;
        case 'r':
            sched = USO_ROUND_ROBIN;
            break;
        default:
            break;
        }
        cli->prio = prio;
        cli->sched = sched;
    }
    else
    {
        char *prio;
        char *sched;
        switch (cli->prio)
        {
        case USO_USER:
            prio = "usr";
            break;
        case USO_SYSTEM:
            prio = "sys";
            break;
        case USO_INTERRUPT:
            prio = "int";
            break;
        default:
            prio = "?";
            break;
        }
        switch (cli->sched)
        {
        case USO_FIFO:
            sched = "fifo";
            break;
        case USO_ROUND_ROBIN:
            sched = "roro";
            break;
        default:
            sched = "?";
            break;
        }
        ACE_printf ("cli flags: prio=%s sched=%s\n", prio, sched);
    }
    return err;
}


