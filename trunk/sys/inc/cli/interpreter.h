/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include <cli/exec.h>
#include <cli/parser.h>
#include <mfs/vfs.h>

/** @addtogroup cli
 * @{
 */

/** @defgroup interpreter interpreter.h
 *
 * Command line interpreter.
 * @{
 */

/** Maximal size for the command line. */
#define CLI_LINE_SIZE         256

struct CLI_interpreter
{
    CLI_parser_t p;
    MFS_descriptor_t *out_desc;
    MFS_descriptor_t *in_desc;
    char line_buffer[CLI_LINE_SIZE];
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

/**
 *  This function is the entry function for the CLI thread.
 *  @param cli : CLI_interpreter_t passed as argument to the entry function.
 */
extern ACE_err_t CLI_interpreter_run (void *cli);

/** @}
 */

/** @}
 */

#endif
