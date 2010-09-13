/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_LOG_H
#define USO_LOG_H

#include "ace/stdio.h"

/** @defgroup kprintf kprintf.h
 *
 * Kernel log.
 *
 * @{
 */

/*------------- Representation ------------------------------------------*/

/**
 * Kernel log levels.
 */
enum USO_log_level
{

    /** Show only error messages when the system gets out of control */
    USO_LL_PANIC,

    /** Show all error messages */
    USO_LL_ERROR,

    /** Show all error and warnings */
    USO_LL_WARNING,

    /** Show error, warning and info messages. */
    USO_LL_INFO,

    /** Also show debugging messages. */
    USO_LL_DEBUG,

    /** Show all messages */
    USO_LL_PROTOCOL
};

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize kprintf.
 *
 * @param ios : IO Stream for output.
 * @param level : Default log level.
 */
extern void USO_log_init (ACE_FILE * ios, enum USO_log_level level);

/**
 * Show log level.
 *
 */
extern void USO_log_show (void);

/**
 * Increase log level.
 *
 * For more Output.
 */
extern void USO_log_inc (void);

/**
 * Decrease log level.
 *
 * For lesser Output.
 */
extern void USO_log_dec (void);


/**
 * Put string to kernel log output.
 *
 * @param level : Print string when log_level >= level.
 * @param string : Pointer to string.
 */
extern void USO_kputs (enum USO_log_level level, char *string);

/**
 * Print formatted string to kernel log output.
 *
 * @param level : Print string when log_level >= level.
 * @param fmt : Pointer to formatted string.
 */
extern void USO_kprintf (enum USO_log_level level, const char *fmt, ...);

/**
 * Print formatted debug message to kernel log output.
 * 
 * Log Level is DEBUG.
 *
 * @param fmt : Pointer to formatted string.
 */
extern void USO_debug (const char *fmt, ...);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
