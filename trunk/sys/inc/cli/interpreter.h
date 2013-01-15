/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include <cli/exec.h>
#include <mfs/vfs.h>

/** @addtogroup cli
 * @{
 */

/** @defgroup interpreter interpreter.h
 *
 * Command line interpreter.
 * @{
 */

extern char CLI_err_arg_missing[];
extern char CLI_err_arg_notfound[];
extern char CLI_err_type_inval[];
extern char CLI_err_open_fail[];
extern char CLI_err_cmd_fail[];
extern char CLI_err_sys_fail[];

/** Maximal token size for the CLI. */
#define CLI_TOKEN_SIZE (ACE_MAX(MFS_PATH_SIZE,32))
/** Maximal amount of tokens for the CLI. */
#define CLI_TOKEN_COUNTER 4

struct CLI_interpreter
{
    MFS_descriptor_t *desc;
    MFS_descriptor_t *out_desc;
    MFS_descriptor_t *in_desc;
    MFS_descriptor_t *exe_desc;
    char token_buffer[CLI_TOKEN_COUNTER][CLI_TOKEN_SIZE];
    char *argv[CLI_TOKEN_COUNTER];
    int argc;
    int prio;
    int sched;
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
extern void CLI_interpreter_init (CLI_interpreter_t * cli);

extern MFS_descriptor_t *CLI_get_dir(CLI_interpreter_t * cli);

/**
 *  This function is the entry function for the CLI thread.
 *  @param cli : CLI_interpreter_t passed as argument to the entry function.
 */
extern void CLI_interpreter_run (void *cli);

/** @}
 */

/** @}
 */

#endif
