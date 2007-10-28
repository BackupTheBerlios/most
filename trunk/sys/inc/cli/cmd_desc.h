/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef CLI_CMD_DESC_H
#define CLI_CMD_DESC_H

#include "cli/interpreter.h"

/** @addtogroup basic_commands
 *
 * @{
 */

/**
 * List commands.
 *
 */
extern bool_t CLI_cmd_help (CLI_interpreter_t *cli);

/**
 * Descriptor open.
 *
 */
extern bool_t CLI_cmd_open (CLI_interpreter_t *cli);

/**
 * Descriptor close.
 *
 */
extern bool_t CLI_cmd_close (CLI_interpreter_t *cli);

/**
 * Descriptor information.
 *
 */
extern bool_t CLI_cmd_start (CLI_interpreter_t *cli);

extern bool_t CLI_cmd_stop (CLI_interpreter_t *cli);

extern bool_t CLI_cmd_info (CLI_interpreter_t *cli);

extern bool_t CLI_cmd_list (CLI_interpreter_t *cli);

extern bool_t CLI_cmd_history (CLI_interpreter_t *cli);

extern bool_t CLI_cmd_exec (CLI_interpreter_t *cli);

extern bool_t CLI_cmd_run (CLI_interpreter_t *cli);

/**
 * Change LogLevel.
 * @param cli Comand line interpreter
 */
extern bool_t CLI_cmd_klog (CLI_interpreter_t *cli);

/** @}
 */

#endif
