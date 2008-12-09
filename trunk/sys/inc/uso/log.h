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
 * Kernel log levels
 */
enum USO_log_level
{

    /** Show only error messages when the system gets out of control */
    USO_LL_PANIC,

    /** Show all error messages */
    USO_LL_ERROR,

    /** Show all error and warnings */
    USO_LL_WARNING,

    /** info message level */
    USO_LL_INFO,

    /** level for debugging */
    USO_LL_DEBUG,

    /** show also messages for protokolling what happens */
    USO_LL_PROTOCOL
};

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize kprintf.
 *
 * @param ios : IO Stream for output.
 * @param level : default log level.
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
 * For less Output.
 */
extern void USO_log_dec (void);


/**
 * Put string to kernel log output.
 *
 * @param level : print when log_level >= level.
 * @param string : Pointer to string.
 */
extern void USO_kputs (enum USO_log_level level, char *string);

/**
 * Format print string to kernel log output.
 *
 * @param level : print when log_level >= level.
 * @param fmt : Pointer to formated string.
 */
extern void USO_kprintf (enum USO_log_level level, const char *fmt, ...);

/**
 * Format debug to kernel log output.
 * 
 * Log Level is DEBUG.
 *
 * @param fmt : Pointer to formated string.
 */
extern void USO_debug (const char *fmt, ...);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
