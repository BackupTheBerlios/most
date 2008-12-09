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
 * Command line interpreter.
 * @{
 */
struct CLI_command
{
    char *description;
    ACE_bool_t (*f) (CLI_interpreter_t *);
};

/** Command type */
typedef struct CLI_command CLI_command_t;

/**
 *  Initialize and install a command.
 *  
 *  @param command pointer to a CLI_command_t structure
 *  @param name pointer to command name
 *  @param desc pointer to a short description
 *  @param f  function is executed if you run the command 
 */
extern void CLI_command_init (CLI_command_t * command,
	 	              char *name, char *desc, ACE_bool_t (*f) (CLI_interpreter_t *));

/**
 *  Initialize and install some basic commands
 *  
 */
extern void CLI_commands_init (void);


struct CLI_exec
{
    char *description;
    void (*f) (char *);
};

typedef struct CLI_exec CLI_exec_t;

extern void CLI_exec_init (MFS_descriptor_t * dir, CLI_exec_t * exec,
                  char *name, char *description, void (*f) (char *));

/** @}
 */

#endif
