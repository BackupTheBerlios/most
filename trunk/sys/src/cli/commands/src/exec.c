/*
 * exec.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <uso/scheduler.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <cli/command.h>
#include <cli/exec.h>
#include <mfs/descriptor.h>

extern ACE_bool_t
CLI_cmd_exec (CLI_interpreter_t * cli)
{
    ACE_bool_t done = FALSE;
    CLI_exec_t *exec = NULL;
    if (cli->exe_desc != NULL)
    {
    	exec = (CLI_exec_t *)cli->exe_desc->represent;
    }
    else if (USO_thread_dir_get(USO_current())->type == MFS_EXEC)
    {
        exec = (CLI_exec_t *) USO_thread_dir_get(USO_current())->represent;
    }
    if (exec != NULL)
    {
    	ACE_FILE *in_tmp = NULL;
    	ACE_FILE *out_tmp = NULL;
    	if (cli->out_desc != NULL){
            if (MFS_open_desc (cli->out_desc) == ACE_OK){
            	out_tmp = USO_current()->out;
            	USO_thread_out_init (USO_current(), cli->out_desc);
            }
    	}
    	if (cli->in_desc != NULL){
            if (MFS_open_desc (cli->in_desc) == ACE_OK){
            	in_tmp = USO_current()->in;
            	USO_thread_in_init (USO_current(), cli->in_desc);
            }
    	}
        char *arg = NULL;
        if (cli->argc >= 1)
        {
            arg = cli->argv[0];
        }
        exec->f (arg);
    	if (out_tmp != NULL){
    		USO_thread_out_init (USO_current(), out_tmp);
    		MFS_close_desc(cli->out_desc);
    	}
    	if (in_tmp != NULL){
    		USO_thread_in_init (USO_current(), in_tmp);
    		MFS_close_desc(cli->in_desc);
    	}
        done = TRUE;
    }
    else
    {
        ACE_puts (CLI_err_type_inval);
    }
    return done;
}

