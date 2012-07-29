/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#include <uso/log.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/thread.h>
#include <uso/heap.h>
#include <cli/commands.h>
#include <cli/cmd_desc.h>

#include "mfs/descriptor.h"
#include "mfs/directory.h"
#include "mfs/sysfs.h"

#define CLI_RUN_STACK_SIZE  (0x400/sizeof(USO_stack_t))

static char arg_missing[] = "arg missing.\n";
static char arg_notfound[] = "arg not found.\n";
static char type_inval[] = "type invalid.\n";
static char sys_fail[] = "sys fail.\n";

extern ACE_bool_t
CLI_cmd_open (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = cli->desc;
    if (cli->argc >= 1)
    {
        desc = MFS_open (desc, cli->argv[0]);
        if (desc != NULL)
        {
            cli->desc = desc;
            done = TRUE;
        }
        else
        {
            ACE_puts (arg_notfound);
        }
    }
    else
    {
        ACE_puts (arg_missing);
    }
    return done;
}

extern ACE_bool_t
CLI_cmd_close (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc;
    desc = MFS_close_desc (cli->desc);
    if (desc != NULL)
    {
        cli->desc = desc;
        done = TRUE;
    }
    else
    {
        ACE_puts (sys_fail);
    }
    return done;
}

extern ACE_bool_t
CLI_cmd_start (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = MFS_sysfs_get_dir (MFS_SYSFS_DIR_THREADS);
    if (cli->argc >= 1)
    {
        desc = MFS_lookup (desc, cli->argv[0]);
        if (desc != NULL)
        {
            USO_start ((USO_thread_t *) desc->entry);
            done = TRUE;
        }
        else
        {
            ACE_puts (arg_notfound);
        }
    }
    else
    {
        ACE_puts (arg_missing);
    }
    return done;
}

extern ACE_bool_t
CLI_cmd_stop (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = MFS_sysfs_get_dir (MFS_SYSFS_DIR_THREADS);
    if (cli->argc >= 1)
    {
        desc = MFS_lookup (desc, cli->argv[0]);
        if (desc != NULL)
        {
            USO_stop ((USO_thread_t *) desc->entry);
            done = TRUE;
        }
        else
        {
            ACE_puts (arg_notfound);
        }
    }
    else
    {
        ACE_puts (arg_missing);
    }
    return done;
}

extern ACE_bool_t
CLI_cmd_info (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    MFS_descriptor_t *desc = cli->desc;
    if (cli->argc >= 1)
    {
        desc = MFS_lookup (desc, cli->argv[0]);
        if (desc != NULL)
        {
            MFS_info_desc (desc);
            done = TRUE;
        }
        else
        {
            ACE_puts (arg_notfound);
        }
    }
    else
    {
        MFS_info_desc (desc);
        done = TRUE;
    }
    return done;
}


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
            USO_thread_info_head ();
            desc = MFS_sysfs_get_dir (MFS_SYSFS_DIR_THREADS);
            break;
        case 'h':
            info = TRUE;
            USO_heap_info_head ();
            desc = MFS_sysfs_get_dir (MFS_SYSFS_DIR_HEAPS);
            break;
        case 'c':
            info = TRUE;
            CLI_command_info_head ();
            desc = MFS_sysfs_get_dir (MFS_SYSFS_DIR_CMD);
            break;
        default:
            break;
        }
    }
    MFS_descriptor_t *iterator = NULL;
    while ((iterator = MFS_next_entry (desc, iterator)) != NULL)
    {
        MFS_descriptor_info (iterator);
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

extern ACE_bool_t
CLI_cmd_exec (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    if (cli->desc->type == MFS_EXEC)
    {
        CLI_exec_t *exec = (CLI_exec_t *) cli->desc->entry;
        char *arg = NULL;
        if (cli->argc >= 1)
        {
            arg = cli->argv[0];
        }
        exec->f (arg);
        done = TRUE;
    }
    else
    {
        ACE_puts (type_inval);
    }
    return done;
}

extern ACE_bool_t
CLI_cmd_run (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    if (cli->desc->type == MFS_EXEC)
    {
        CLI_exec_t *exec = (CLI_exec_t *) cli->desc->entry;
        char *arg = NULL;
        int prio = USO_USER;
        int sched = USO_ROUND_ROBIN;
        if (cli->argc >= 1)
        {
            switch (cli->argv[0][0])
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
            switch (cli->argv[0][1])
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
        }
        else
        {
            ACE_puts (arg_missing);
            return done;
        }
        USO_thread_t *t = USO_thread_new ((void (*)(void *))exec->f,
                                          CLI_RUN_STACK_SIZE, prio, sched, cli->desc->name);
        if (t != NULL)
        {
            USO_thread_flags_set (t, 1 << USO_FLAG_DETACH);
            if (cli->argc >= 2)
            {
                ACE_size_t len = ACE_strlen (cli->argv[1]) + 1;
                arg = ACE_malloc (len);
                if (arg)
                {
                    memcpy (arg, cli->argv[1], len);
                    USO_thread_arg_init (t, arg);
                    USO_thread_flags_set (t, 1 << USO_FLAG_FREE_ARG);
                }
            }
            USO_start (t);
            done = TRUE;
        }
        else
        {
            ACE_puts (sys_fail);
        }
    }
    else
    {
        ACE_puts (type_inval);
    }
    return done;
}

extern ACE_bool_t
CLI_cmd_klog (CLI_interpreter_t * cli)
{
    if (cli->argc <= 0)
    {
        USO_log_show ();
    }
    else
    {
        switch (cli->argv[0][0])
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
    }
    return TRUE;
}
