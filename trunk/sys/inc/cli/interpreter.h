/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include "mfs/vfs.h"

/** @defgroup interpreter interpreter.h
 *
 * Command line interpreter.
 * @{
 */

/** Maximal token size for the CLI. */
#define CLI_TOKEN_SIZE 16
/** Maximal amount of tokens for the CLI. */
#define CLI_TOKEN_COUNTER 4

struct CLI_interpreter
{
	MFS_descriptor_t *desc;
	char token_buffer[CLI_TOKEN_COUNTER][CLI_TOKEN_SIZE];
	char *argv[CLI_TOKEN_COUNTER];
	int argc;
};

/** CLI_interpreter_t. */
typedef struct CLI_interpreter CLI_interpreter_t;

/**
 *  Set hostname.
 *  
 *  @param name : Hostname(printed at the promt).
 */
extern void CLI_setup (const char *name);

/**
 *  Initialize command line interpreter.
 *  
 *  @param cli : Pointer to Command line interpreter.
 */
extern void CLI_interpreter_init (CLI_interpreter_t *cli);

/**
 *  This function is the entry function for the CLI thread.
 *  @param cli : CLI_interpreter_t passed as argument to the entry function.
 */
extern void CLI_interpreter_run (void *cli);

/** @}
 */

#endif
