/*
 * run.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/thread.h>
#include <uso/heap.h>
#include <cli/command.h>
#include <cli/exec.h>

#define CLI_RUN_STACK_SIZE  (0x400/sizeof(USO_stack_t))

extern ACE_bool_t
CLI_cmd_run (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    CLI_exec_t *exec = NULL;
    char *name;
    if (cli->exe_desc != NULL)
    {
    	exec = (CLI_exec_t *)cli->exe_desc->represent;
    	name = cli->exe_desc->name;
    }
    else if (cli->desc->type == MFS_EXEC)
    {
        exec = (CLI_exec_t *) cli->desc->represent;
        name = cli->desc->name;
    }

    if (exec != NULL)
    {
        USO_thread_t *t = USO_thread_new ((void (*)(void *))exec->f,
                                          CLI_RUN_STACK_SIZE, cli->prio, cli->sched, name);
        if (t != NULL)
        {
            USO_thread_cli_init (t, cli);
        	if (cli->out_desc != NULL){
                if (MFS_open_desc (cli->out_desc) == ACE_OK){
                	USO_thread_out_init (t, cli->out_desc);
                	USO_thread_flags_set (t, 1 << USO_FLAG_CLOSE_OUT);
                }
        	}
        	if (cli->in_desc != NULL){
                if (MFS_open_desc (cli->in_desc) == ACE_OK){
                	USO_thread_in_init (t, cli->in_desc);
                	USO_thread_flags_set (t, 1 << USO_FLAG_CLOSE_IN);
                }
        	}
            USO_thread_flags_set (t, 1 << USO_FLAG_DETACH);
            if (cli->argc >= 1)
            {
                ACE_size_t len = ACE_strlen (cli->argv[1]) + 1;
                char *arg = ACE_malloc (len);
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
            ACE_puts (CLI_err_sys_fail);
        }
    }
    else
    {
        ACE_puts (CLI_err_type_inval);
    }
    return done;
}


