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
    int run_stack_size;
};

/** CLI_interpreter_t. */
typedef struct CLI_interpreter CLI_interpreter_t;

/**
 *  Set hostname.
 *  
 *  @param name : Hostname(printed at the promt).
 */
extern void CLI_setup (const char *name);


extern ACE_err_t CLI_interpreter_start (char *name, MFS_descriptor_t *tty, int stack_size, int run_stack_size);

/** @}
 */

/** @}
 */

#endif
