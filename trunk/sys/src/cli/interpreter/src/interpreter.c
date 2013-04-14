/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/ctype.h>
#include <ace/stdlib.h>
#include <uso/list.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/interpreter.h>
#include <cli/command.h>
#include <cli/exec.h>
#include <cli/pipe.h>

char CLI_err_arg_missing[] = "arg missing.\n";
char CLI_err_arg_notfound[] = "arg not found.\n";
char CLI_err_type_inval[] = "type invalid.\n";
char CLI_err_open_fail[] = "open fail.\n";
char CLI_err_cmd_fail[] = "cmd fail.\n";
char CLI_err_sys_fail[] = "sys fail.\n";

#define CLI_PIPE_BUF_SIZE (512)

enum CLI_state
{
	CLI_STATE_NORMAL,
	CLI_STATE_SUB,
	CLI_STATE_EXEC
};

#define CLI_RX_POLLING_TIME USO_MSEC_2_TICKS(250)

static const char *hostname;

extern void
CLI_setup (const char *name)
{
    hostname = name;
    CLI_commands_install ();
    CLI_executes_install ();
}

extern void
CLI_interpreter_init (CLI_interpreter_t * cli)
{
    cli->exe_desc = NULL;
    cli->prio = USO_USER;
    cli->sched = USO_ROUND_ROBIN;
}

static void
promt (CLI_interpreter_t * cli)
{
    ACE_printf ("%s:%s# ", hostname, USO_thread_dir_get(USO_current())->name);
}



static int
parse (CLI_interpreter_t * cli, char *buf)
{
    char *token = buf;
    ++cli->argc;
    for (;;)
    {
        int c = ACE_getc ();
        if (c == ACE_EOF)
        {
            USO_sleep (CLI_RX_POLLING_TIME);
        }
        else
        {
            if (ACE_isprint (c))
            {
                ACE_putc ((unsigned char)c);
            }
            if (c == '\n')
            {
                *token = '\0';
                return (token - buf);
            }
            else if (c == '\b')
            {
                if (token > buf)
                {
                    ACE_putc ((unsigned char)c);
                    ACE_putc ((unsigned char)' ');
                    ACE_putc ((unsigned char)c);
                    token--;
                }
                else if (cli->argc > 1)
                {
                    cli->argc--;
                    return (-1);
                }
            }
            else if (ACE_isgraph (c))
            {
                if (token < buf + CLI_TOKEN_SIZE - 1)
                {
                    *token++ = c;
                }
            }
            else if (c == ' ')
            {
                *token = '\0';
                if (cli->argc < CLI_TOKEN_COUNTER)
                {
                    if (parse (cli, cli->token_buffer[cli->argc]) < 0)
                        ACE_putc ((unsigned char)'\b');
                    else
                        return (token - buf);
                }
                else
                    return (token - buf);
            }
        }
    }
    return 0;
}

static int
inc_argv (CLI_interpreter_t * cli, int idx)
{
    if (cli->argc > 0)
    {
        --cli->argc;
    }
    ++idx;
    for (int i = 0; i < cli->argc; ++i)
    {
        cli->argv[i] = cli->token_buffer[idx + i];
    }
    return idx;
}

extern void
CLI_interpreter_run (void *param)
{
    CLI_interpreter_t *cli = (CLI_interpreter_t *) param;
    CLI_command_t *command;
    int idx;
    enum CLI_state state = CLI_STATE_NORMAL;
    USO_thread_dir_set (USO_current(), MFS_get_root());
    USO_log_puts (USO_LL_INFO, "Cli is running.\n");
    for (;;)
    {
    	cli->out_desc = NULL;
    	cli->in_desc = NULL;
    	state = CLI_STATE_NORMAL;
        for (int i = 0; i < CLI_TOKEN_COUNTER; ++i)
        {
            cli->token_buffer[i][0] = '\0';
            cli->argv[i] = cli->token_buffer[i];
        }
        cli->argc = 0;
        idx = 0;

        promt (cli);
        (void)parse (cli, cli->token_buffer[cli->argc]);
        ACE_putc ('\n');

        if (ACE_strlen (cli->argv[0]) <= 0)
            continue;

        if (cli->argv[0][0] == '#')
        {
            if (ACE_strlen (cli->argv[0]) >= 3){
            	ACE_putc (ACE_atoxc (&cli->argv[0][1]));
            }
        	continue;
        }

        if (cli->argv[0][0] == '('){
        	if (ACE_strlen (cli->argv[0]) >= 2){
        		if ( USO_thread_dir_get(USO_current())->type == MFS_DIRECTORY ){
        			CLI_pipe_new (CLI_PIPE_BUF_SIZE, USO_thread_dir_get(USO_current()), &cli->argv[0][1]);
        		}
        	} else {
                ACE_puts (CLI_err_arg_missing);
        	}
        	continue;
        }

        if (cli->argv[0][0] == ')'){
        	if (ACE_strlen (cli->argv[0]) >= 2){
        		CLI_pipe_del(USO_thread_dir_get(USO_current()), &cli->argv[0][1]);
        	} else {
                ACE_puts (CLI_err_arg_missing);
        	}
        	continue;
        }

        if (cli->argv[0][0] == '<'){
        	if (ACE_strlen (cli->argv[0]) >= 2){
                MFS_descriptor_t *desc = MFS_lookup (USO_thread_dir_get(USO_current()), &cli->argv[0][1]);
                if (desc != NULL && desc->type == MFS_STREAM)
                {
                    cli->in_desc = desc;
                } else {
                    ACE_puts (CLI_err_arg_notfound);
                    continue;
                }
        	} else {
                ACE_puts (CLI_err_arg_missing);
            	continue;
        	}
            idx = inc_argv (cli, idx);
        }

        if (cli->argv[0][0] == '>'){
        	if (ACE_strlen (cli->argv[0]) >= 2){
                MFS_descriptor_t *desc = MFS_lookup (USO_thread_dir_get(USO_current()), &cli->argv[0][1]);
                if (desc != NULL && desc->type == MFS_STREAM)
                {
                    cli->out_desc = desc;
                } else {
                    ACE_puts (CLI_err_arg_notfound);
                    continue;
                }
        	} else {
                ACE_puts (CLI_err_arg_missing);
            	continue;
        	}
            idx = inc_argv (cli, idx);
        }

        if (cli->argv[0][0] == '.')
        {
            cli->argv[0] = &cli->argv[0][1];
            if (CLI_cmd_open (cli) == FALSE)
            {
                ACE_puts (CLI_err_open_fail);
                continue;
            }
            state = CLI_STATE_SUB;
            idx = inc_argv (cli, idx);
        }

        command = CLI_find_cmd (cli->argv[0]);
        if ( (command == NULL) && (state == CLI_STATE_NORMAL) )
        {
            if (cli->argv[0][0] == '&')
            {
                cli->argv[0] = &cli->argv[0][1];
                command = CLI_find_cmd ("run");
            }
            else
            {
                command = CLI_find_cmd ("exec");
            }
            cli->exe_desc = CLI_find_exe(cli->argv[0]);
            if (cli->exe_desc == NULL){
            	if (CLI_cmd_open (cli) == FALSE)
            	{
            		ACE_puts (CLI_err_open_fail);
            		continue;
            	}
            }
            state = CLI_STATE_EXEC;
        }

        idx = inc_argv (cli, idx);
        if (command != NULL)
        {
            if (command->f (cli) == FALSE)
            {
                ACE_puts (CLI_err_cmd_fail);
            }
        }
        else
        {
            ACE_puts (CLI_err_sys_fail);
        }
        if (state == CLI_STATE_EXEC)
        {
        	if (cli->exe_desc == NULL)
        		CLI_cmd_close (cli);
        	else
        		cli->exe_desc = NULL;
        }
        else if (state == CLI_STATE_SUB)
        {
            CLI_cmd_close (cli);
        }

    }
}
