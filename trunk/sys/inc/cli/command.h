/*
 * command.h
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <mfs/vfs.h>
#include <cli/interpreter.h>

/** @addtogroup cli
 * @{
 */

/** @defgroup command command.h
 *
 * Register commands and executables.
 * Commands are called from the CLI.
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
 *  Initialize and install all basic commands.
 *
 */
extern void CLI_commands_install (void);

extern CLI_command_t *CLI_find_cmd (char* name);

/**
 * Open a descriptor.
 * @param cli : Pointer to CLI.
 * @return TRUE if open was successful else FALSE
 */
extern ACE_bool_t CLI_cmd_open (CLI_interpreter_t * cli);

/**
 * Close a descriptor.
 * @param cli : Pointer to CLI.
 * @return TRUE if close was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_close (CLI_interpreter_t * cli);


/**
 * Change Directory.
 * @param cli : Pointer to CLI.
 * @return TRUE if open was successful else FALSE
 */
extern ACE_bool_t CLI_cmd_cd (CLI_interpreter_t * cli);

/**
 * Start a stopped thread.
 * @param cli : Pointer to CLI.
 * @return TRUE if start was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_start (CLI_interpreter_t * cli);

/**
 * Stop a started thread.
 * @param cli : Pointer to CLI.
 * @return TRUE if stop was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_stop (CLI_interpreter_t * cli);

/**
 * Proint information about current descriptor.
 * @param cli : Pointer to CLI.
 * @return TRUE if info was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_info (CLI_interpreter_t * cli);

/**
 * List current content of descriptor if descriptor is a directory.
 * @param cli : Pointer to CLI.
 * @return TRUE if list was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_list (CLI_interpreter_t * cli);

/**
 * Execute a command.
 * You can register C function as command.
 * @param cli : Pointer to CLI.
 * @return TRUE if exec was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_exec (CLI_interpreter_t * cli);

/**
 * Run a command in a new created thread.
 * @param cli : Pointer to CLI.
 * @return TRUE if run was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_run (CLI_interpreter_t * cli);

/**
 * Change or show the log level.
 * @param cli : Pointer to CLI.
 * @return TRUE if klog was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_klog (CLI_interpreter_t * cli);

extern ACE_bool_t CLI_cmd_run_flag (CLI_interpreter_t * cli);

extern ACE_bool_t CLI_cmd_remove (CLI_interpreter_t * cli);

/** @}
 */

/** @}
 */


#endif /* COMMAND_H_ */
