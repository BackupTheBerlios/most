/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/list.h>
#include <uso/heap.h>
#include <ace/stdio.h>

#include "cli/commands.h"
#include "cli/cmd_desc.h"
#include "mfs/vfs.h"
#include "mfs/directory.h"

USO_list_t CLI_commands;

static CLI_command_t help;
static CLI_command_t open;
static CLI_command_t close;
static CLI_command_t start;
static CLI_command_t stop;
static CLI_command_t info;
static CLI_command_t list;
static CLI_command_t exec;
static CLI_command_t run;
static CLI_command_t klog;

extern void
CLI_command_init (CLI_command_t * command,
                  char *name, char *description, bool_t (*f) (CLI_interpreter_t *) )
{
    command->name = name;
    command->description = description;
    command->f = f;
    USO_enqueue (&CLI_commands, (USO_node_t *) command);
}

extern void
CLI_commands_init (void)
{
    USO_list_init (&CLI_commands);
    CLI_command_init (&help, "help", "List commands", CLI_cmd_help);
    CLI_command_init (&open, "open", "Descriptor open", CLI_cmd_open);
    CLI_command_init (&close, "close", "Descriptor close", CLI_cmd_close);
    CLI_command_init (&start, "start", "Thread start", CLI_cmd_start);
    CLI_command_init (&stop, "stop", "Thread stop", CLI_cmd_stop);
    CLI_command_init (&info, "info", "Descriptor information", CLI_cmd_info);
    CLI_command_init (&list, "list", "List directory", CLI_cmd_list);
    CLI_command_init (&exec, "exec", "Execute descriptor", CLI_cmd_exec);
    CLI_command_init (&run, "run", "Run descriptor", CLI_cmd_run);
    CLI_command_init (&klog, "klog", "Kenel log level", CLI_cmd_klog);
}

extern void
exec_info(MFS_entry_t *entry)
{
	CLI_exec_t * exec = (CLI_exec_t *) entry;
	printf("[ %s ].\n", exec->description);
}

static struct MFS_descriptor_op exec_descriptor_op = {.open = NULL,
								        		      .close = NULL,
										              .info = exec_info};


extern void
CLI_exec_init (MFS_descriptor_t * dir, CLI_exec_t * exec,
                  char *name, char *description, void (*f) (char *))
{
    exec->description = description;
    exec->f = f;
	MFS_create_unknowen(dir, name, (MFS_entry_t*) exec, MFS_EXEC, &exec_descriptor_op);
}
