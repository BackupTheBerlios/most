/*
 * list.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_bool_t
CLI_cmd_list (CLI_interpreter_t * cli)
{
    ACE_bool_t info = FALSE;
    MFS_descriptor_t *desc = cli->desc;
    if (cli->argc >= 1)
    {
        switch (cli->argv[0][0])
        {
        case 'i':
            info = TRUE;
            break;
        case 't':
            info = TRUE;
            desc = MFS_resolve(MFS_get_root(), "sys/uso/thread");
            break;
        case 'm':
            info = TRUE;
            desc = MFS_resolve(MFS_get_root(), "sys/uso/heap");
            break;
        case 'c':
            info = TRUE;
            desc = MFS_resolve(MFS_get_root(), "sys/cli/cmd");
            break;
        case 'e':
            info = TRUE;
            desc = MFS_resolve(MFS_get_root(), "sys/cli/exe");
            break;
        case 'a':
            info = TRUE;
            desc = MFS_resolve(MFS_get_root(), "sys/dev/clock/timer");
            break;
        default:
            break;
        }
    }
    MFS_descriptor_t *iterator = NULL;
    while ((iterator = MFS_next_entry (desc, iterator)) != NULL)
    {
        MFS_descriptor_print (iterator);
        if (info == TRUE)
        {
            MFS_info_desc (iterator);
        }
        else
        {
            ACE_putc ('\n');
        }
    }
    return TRUE;
}

