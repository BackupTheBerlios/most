/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_COMMANDS_H
#define CLI_COMMANDS_H

#include "mfs/vfs.h"
#include "cli/interpreter.h"

/** @defgroup commands commands.h
 *
 * Register commands and executables.
 * Commands are called from the CLI.
 * Executeables are registered  C function which you can
 * execute with the commands exec and run.
 * @{
 */
struct CLI_command
{
    char *description;
    ACE_bool_t (*f) (CLI_interpreter_t *);
};

/** Command type. */
typedef struct CLI_command CLI_command_t;

/**
 *  Print command info header.
 *
 */
extern void CLI_command_info_head(void);

/**
 *  Initialize and install a command.
 *  Commands are installed in the cli directory(descriptor).
 *  
 *  @param command : Pointer to a CLI_command_t.
 *  @param name : Pointer to command name.
 *  @param desc :  Pointer to a short description of the command.
 *  @param f : Function which will be executed if you type the command name in the CLI.
 */
extern void CLI_command_init (CLI_command_t * command,
	 	              char *name, char *desc, ACE_bool_t (*f) (CLI_interpreter_t *));

/**
 *  Initialize and install all basic commands in the cmd_desc.h module.
 *  
 */
extern void CLI_commands_init (void);


struct CLI_exec
{
    char *description;
    void (*f) (char *);
};

/** Executeable type. */
typedef struct CLI_exec CLI_exec_t;

/**
 *  Initialize and install an C function as executeable.
 * 	This function will be executed with the commands exec or run.
 *  An argument may be passed to the function.
 *
 *  @param dir : Directory descriptor to which the executeable will be installed.
 *  @param exec : Pointer to a CLI_exec_t.
 *  @param name : Pointer to name of executeable.
 *  @param desc :  Pointer to a short description of the executeable.
 *  @param f : Function which will be executed with the commands exec or run.
 */
extern void CLI_exec_init (MFS_descriptor_t * dir, CLI_exec_t * exec,
                  char *name, char *description, void (*f) (char *));

/** @}
 */

#endif
