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
#include <cli/err.h>
#include <cli/interpreter.h>
#include <cli/read_line.h>
#include <cli/parser.h>
#include <cli/command.h>
#include <cli/exec.h>
#include <cli/pipe.h>
#include <cli/debug.h>

static const char *hostname;

extern void
CLI_setup (const char *name)
{
    hostname = name;
    CLI_commands_install ();
    CLI_executes_init ();
}

static void
interpreter_init (CLI_interpreter_t * cli, int run_stack_size)
{
    cli->in_desc = NULL;
    cli->out_desc = NULL;
    cli->prio = USO_USER;
    cli->sched = USO_ROUND_ROBIN;
    cli->run_stack_size = run_stack_size;
}

static void
promt (CLI_interpreter_t * cli)
{
    ACE_printf ("%s_%s:%s# ", USO_current()->desc->name, hostname, USO_thread_work_get(USO_current())->name);
}



static ACE_err_t
interpreter_run (void *arg)
{
    ACE_err_t err;
    CLI_interpreter_t *cli = (CLI_interpreter_t *) arg;
    MFS_descriptor_t *desc = NULL;
    USO_thread_t *t;
    
    USO_thread_work_set (USO_current(), MFS_get_root());
    USO_log_printf (USO_LL_INFO, "%s running.\n", USO_current()->desc->name);

    ACE_putc ('\n');

    for (;;)
    {
        t = NULL;
        promt (cli);

        if ( CLI_read_line(cli->line_buffer, sizeof(cli->line_buffer)) != NULL){
            ACE_putc ('\n');
        } else {
            ACE_puts("\nCLI: read line err\n");
            continue;
        }

        DEBUGF (CLI_INT_DEBUG, ("Cli: cmd line = %s.\n", cli->line_buffer));
        
        if ( (err = CLI_parse (&cli->p, cli->line_buffer)) != ACE_OK){
            if (err != CLI_ERR_LINEEMPTY){
                ACE_printf("\n CLI: syntax err : %i\n", err);
            }
            continue;
        }

        DEBUGF (CLI_INT_DEBUG, ("Cli: in=%s, cmd=%s, arg=(%s) out=%s run=%i.\n",
                        cli->p.in, cli->p.cmd, cli->p.arg, cli->p.out, cli->p.run));

        if (cli->p.in[0] != '\0'){
            desc = MFS_open (USO_thread_work_get(USO_current()), cli->p.in);
            if (desc != NULL && desc->type == MFS_STREAM)
            {
                cli->in_desc = USO_current()->in;
                USO_thread_in_init (USO_current(), desc);
            } else {
                ACE_printf ("CLI: in %s not found\n", cli->p.in);
                continue;
            }
        }

        if (cli->p.out[0] != '\0'){
            desc = MFS_open (USO_thread_work_get(USO_current()), cli->p.out);
            if (desc != NULL && desc->type == MFS_STREAM)
            {
                cli->out_desc = USO_current()->out;
                USO_thread_out_init (USO_current(), desc);
            } else {
                ACE_printf ("CLI: out %s not found\n", cli->p.out);
                continue;
            }
        }

        desc = CLI_find_cmd (cli->p.cmd);
        if (desc != NULL){
            CLI_command_t *cmd = (CLI_command_t *) desc->represent;
            err = cmd->f (cli);
            if (err < ACE_OK)
            {
                ACE_printf ("CLI: cmd fail: %i\n", err);
            }
        } else {
            desc = CLI_find_exe(cli->p.cmd);
            if (desc != NULL){
                CLI_exec_t *exec = (CLI_exec_t *) desc->represent;
                if (cli->p.run == FALSE)
                {
                    err = exec->f (cli->p.arg);
                    if (err < ACE_OK)
                    {
                        ACE_printf ("CLI: exe fail: %i\n", err);
                    }
                }
                else
                {
                    t = USO_thread_new ((ACE_err_t (*)(void *))exec->f, cli->run_stack_size, cli->prio, cli->sched, desc->name);
                    if (t != NULL)
                    {
                        if (cli->out_desc != NULL){
                            USO_thread_flags_set (t, 1 << USO_FLAG_CLOSE_OUT);
                        }
                        if (cli->in_desc != NULL){
                            USO_thread_flags_set (t, 1 << USO_FLAG_CLOSE_IN);
                        }
                        USO_thread_flags_set (t, 1 << USO_FLAG_DETACH);
                        ACE_size_t len = ACE_strlen (cli->p.arg) + 1;
                        char *arg = ACE_malloc (len);
                        if (arg)
                        {
                            memcpy (arg, cli->p.arg, len);
                            USO_thread_arg_init (t, arg);
                            USO_thread_flags_set (t, 1 << USO_FLAG_FREE_ARG);
                        } else {
                            ACE_puts("CLI: Arg out of mem\n");
                        }
                        USO_start (t);
                    } else {
                        ACE_puts("CLI: Thread out of mem\n");
                    }
                }
                MFS_close_desc(desc); /* also close if exec is run in new thread ? */
            } else {
                ACE_printf ("CLI: cmd %s not found\n", cli->p.cmd);
            }
        }

        if (cli->out_desc != NULL){
            USO_thread_out_init (USO_current(), cli->out_desc);
            if (t == NULL) MFS_close_desc(cli->out_desc);
            cli->out_desc = NULL;
        }

        if (cli->in_desc != NULL){
            USO_thread_in_init (USO_current(), cli->in_desc);
            if (t == NULL) MFS_close_desc(cli->in_desc);
            cli->in_desc = NULL;
        }

    }
    return DEF_ERR_SYS;
}

static CLI_interpreter_t *
interpreter_new(int run_stack_size)
{
    CLI_interpreter_t *cli = ACE_malloc(sizeof(CLI_interpreter_t));
    if (cli != NULL){
        interpreter_init (cli, run_stack_size);
    }
    return cli;
}

extern ACE_err_t
CLI_interpreter_start (char *name, MFS_descriptor_t *tty, int stack_size, int run_stack_size)
{
    ACE_err_t err = ACE_OK;
    CLI_interpreter_t *cli = interpreter_new(run_stack_size);
    if (cli != NULL){
        USO_thread_t *thread = USO_thread_new (interpreter_run,
                        stack_size,
                        USO_USER,
                        USO_ROUND_ROBIN,
                        name);
        if (thread){
            USO_thread_arg_init (thread, cli);
            if (tty != NULL){
                USO_thread_in_init (thread, tty);
                USO_thread_out_init (thread, tty);
            }
            USO_start (thread);
        } else {
            ACE_free(cli);
            err = DEF_ERR_MEM;
        }
    } else {
        err = DEF_ERR_MEM;
    }
    return err;
}
