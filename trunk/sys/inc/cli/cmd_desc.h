/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef CLI_CMD_DESC_H
#define CLI_CMD_DESC_H

#include "cli/interpreter.h"

/** @defgroup cmd_desc cmd_desc.h
 *
 * Command line interpreter commands.
 * @{
 */

/**
 * Open a descriptor.
 * @param cli : Pointer to CLI.
 * @return TRUE if open was successful else FALSE
 */
extern ACE_bool_t CLI_cmd_open (CLI_interpreter_t *cli);

/**
 * Close a descriptor.
 * @param cli : Pointer to CLI.
 * @return TRUE if close was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_close (CLI_interpreter_t *cli);

/**
 * Start a stopped thread.
 * @param cli : Pointer to CLI.
 * @return TRUE if start was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_start (CLI_interpreter_t *cli);

/**
 * Stop a started thread.
 * @param cli : Pointer to CLI.
 * @return TRUE if stop was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_stop (CLI_interpreter_t *cli);

/**
 * Proint information about current descriptor.
 * @param cli : Pointer to CLI.
 * @return TRUE if info was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_info (CLI_interpreter_t *cli);

/**
 * List current content of descriptor if descriptor is a directory.
 * @param cli : Pointer to CLI.
 * @return TRUE if list was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_list (CLI_interpreter_t *cli);

/**
 * Execute a command.
 * You can register C function as command.
 * @param cli : Pointer to CLI.
 * @return TRUE if exec was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_exec (CLI_interpreter_t *cli);

/**
 * Run a command in a new created thread.
 * @param cli : Pointer to CLI.
 * @return TRUE if run was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_run (CLI_interpreter_t *cli);

/**
 * Change or shoe the log level.
 * @param cli : Pointer to CLI.
 * @return TRUE if klog was successful else FALSE.
 */
extern ACE_bool_t CLI_cmd_klog (CLI_interpreter_t *cli);

/** @}
 */

#endif
