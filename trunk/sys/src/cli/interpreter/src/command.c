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
command_info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    CLI_command_t *command = (CLI_command_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "description";
            entry->value.s = command->description;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op command_descriptor_op = {
                .open = NULL,
                .close = NULL,
                .info = command_info,
                .control = NULL,
                .delete = NULL
};

static void
command_init (CLI_command_t * command,
                  char *name, char *description, ACE_err_t (*f) (CLI_interpreter_t *))
{
    command->description = description;
    command->f = f;

    // insert command into fs (directory cli)
    MFS_descriptor_create (cmd_dir, name, MFS_SYS,
                     &command_descriptor_op, (MFS_represent_t *) command);
}

/**
 * The "open" and "close" commands may not be used as "cd" does the same,
 * but i will keep them because this is the basic mechanism for walking threw 
 * the descriptor hierarchy.
 * And yes with cd you can also change to any descriptors not only directories.
 */

static CLI_command_t open;
static CLI_command_t close;
static CLI_command_t info;
static CLI_command_t control;

static CLI_command_t list;
static CLI_command_t cd;
static CLI_command_t create;
static CLI_command_t rename;
static CLI_command_t remove;

static CLI_command_t start;
static CLI_command_t stop;
static CLI_command_t flag;
static CLI_command_t klog;


extern void
CLI_commands_install (void)
{
    cmd_dir = MFS_resolve ("/sys/cli/cmd");
    command_init (&open, "open", "open 'desc'", CLI_cmd_open);
    command_init (&close, "close", "close", CLI_cmd_close);
    command_init (&info, "info", "info ('path')opt", CLI_cmd_info);
    command_init (&control, "ctrl", "ctrl n (l=long,z=size,s=string) value; 0 info", CLI_cmd_control);
    command_init (&list, "ls", "(.'dir')opt ls (i|c|e|t|m|a)opt", CLI_cmd_list);
    command_init (&cd, "cd", "cd 'path'", CLI_cmd_cd);
    command_init (&create, "new", "create new file or directory", CLI_cmd_create);
    command_init (&rename, "ren", "rename file or directory", CLI_cmd_rename);
    command_init (&remove, "del", "delete file or directory", CLI_cmd_remove);
    command_init (&start, "start", "start 'thread'", CLI_cmd_start);
    command_init (&stop, "stop", "stop 'thread'", CLI_cmd_stop);
    command_init (&flag, "flag", "flag ([u|s|i][f|r])opt", CLI_cmd_flag);
    command_init (&klog, "klog", "klog (+|-)opt", CLI_cmd_klog);
}

extern MFS_descriptor_t *
CLI_find_cmd (char* name)
{
    MFS_descriptor_t *desc;
    desc = MFS_lookup (cmd_dir, name);
    if (NULL != desc && desc->type == MFS_SYS)
        return desc;
    return NULL;
}

