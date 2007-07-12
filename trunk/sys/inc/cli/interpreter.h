/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include "mfs/vfs.h"

#define CLI_TOKEN_SIZE 16
#define CLI_TOKEN_COUNTER 4

struct CLI_interpreter
{
	MFS_descriptor_t *desc;
	char token_buffer[CLI_TOKEN_COUNTER][CLI_TOKEN_SIZE];
	char *argv[CLI_TOKEN_COUNTER];
	int argc;
};

typedef struct CLI_interpreter CLI_interpreter_t;

/** @defgroup interpreter interpreter.h
 *
 * Command line interpreter.
 * @{
 */

/**
 *  Set hostname.
 *  
 *  @param name hostname(printed at the promt) 
 */
extern void CLI_setup (const char *name);

/**
 *  Initialize interpreter.
 *  
 *  @param cli Command line interpreter 
 */
extern void CLI_interpreter_init (CLI_interpreter_t *cli);

/**
 *  Interpreter task.
 *  
 */
extern void CLI_interpreter_run (void *cli);

/** @}
 */

#endif
