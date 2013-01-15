/*
 * command.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static MFS_descriptor_t *cmd_dir;

static void
info_head (MFS_descriptor_t * desc)
{
    ACE_printf ("Description\n");
}

static struct MFS_descriptor_op command_head_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info_head,
    .control = NULL
};

static void
command_info_head ()
{
	MFS_descriptor_create (cmd_dir, "command", MFS_INFO, &command_head_descriptor_op, NULL);
}

static void
command_info (MFS_descriptor_t * desc)
{
    CLI_command_t *command = (CLI_command_t *) desc->represent;
    ACE_printf ("[ %s ]\n", command->description);
}

static struct MFS_descriptor_op command_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = command_info,
    .control = NULL
};

static void
command_init (CLI_command_t * command,
                  char *name, char *description, ACE_bool_t (*f) (CLI_interpreter_t *))
{
    command->description = description;
    command->f = f;

    // insert command into fs (directory cli)
    MFS_descriptor_create (cmd_dir, name, MFS_SYS,
                     &command_descriptor_op, (MFS_represent_t *) command);
}

static CLI_command_t open;
static CLI_command_t close;
static CLI_command_t cd;
static CLI_command_t start;
static CLI_command_t stop;
static CLI_command_t info;
static CLI_command_t list;
static CLI_command_t exec;
static CLI_command_t run;
static CLI_command_t klog;
static CLI_command_t run_flag;
static CLI_command_t remove;


extern void
CLI_commands_install (void)
{
	cmd_dir = MFS_resolve (MFS_get_root(), "sys/cli/cmd");
	command_info_head ();
    command_init (&open, "open", "open 'desc'", CLI_cmd_open);
    command_init (&close, "close", "close", CLI_cmd_close);
    command_init (&cd, "cd", "cd 'path'", CLI_cmd_cd);
    command_init (&start, "start", "start 'thread'", CLI_cmd_start);
    command_init (&stop, "stop", "stop 'thread'", CLI_cmd_stop);
    command_init (&info, "info", "info ('path')opt", CLI_cmd_info);
    command_init (&list, "ls", "(.'dir')opt ls (i|c|e|t|m|a)opt", CLI_cmd_list);
    command_init (&exec, "exec", "(.'exec')opt exec ('arg')opt", CLI_cmd_exec);
    command_init (&run, "run", "(.'exec')opt run ('arg')opt", CLI_cmd_run);
    command_init (&klog, "klog", "klog (+|-)opt", CLI_cmd_klog);
    command_init (&run_flag, "runflg", "runflg ([u|s|i][f|r])opt", CLI_cmd_run_flag);
    command_init (&remove, "rm", "remove file or directory", CLI_cmd_remove);
}

extern CLI_command_t *
CLI_find_cmd (char* name)
{
    MFS_descriptor_t *desc;
    desc = MFS_lookup (cmd_dir, name);
    if (NULL != desc && desc->type == MFS_SYS)
    	return (CLI_command_t *) desc->represent;
    return NULL;
}

