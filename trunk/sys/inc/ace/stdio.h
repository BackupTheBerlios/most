/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDIO_H
#define ACE_STDIO_H

#include "uso/list.h"
#include "mfs/vfs.h"

#include "ace/stdarg.h"

/** @defgroup stdio stdio.h
 *
 * Standard io functions.
 *
 * @{
 */

/*---------------- Definitions ------------------------------------------*/

/** End of file. */
#define ACE_EOF (-1)

/** File type. */
typedef MFS_stream_t ACE_FILE;

/*------------------------------------------------------------------------*/

/*---------------- Interface ---------------------------------------------*/

/**
 *  The function behaves exactly as prinf does, but writing its results to a buffer.
 *  The result is terminated with //0.
 *
 *  @param buf : Buffer must have enough space for the result.
 *  @param fmt : Formatted string.
 *
 *  @return Written characters, negativ when an error occur.
 */
extern int ACE_sprintf (char *buf, const char *fmt, ...);

/** vsprintf Equivalent to sprintf, but the variable argumentliste
 *  is replaced with args.
 *  Args has to be initialized with the makro va_start and maybe with calls of va_arg.
 */
extern int ACE_vsprintf (char *buf, const char *fmt, ACE_va_list_t args);


 
/**
 * Initalize list and buffers. Must be called befor you can use the stdio functions.
 */
extern void ACE_stdio_init (void);

/**
 * Read from IO stream.
 *
 * @param in : File descriptor.
 * @param buf : Pointer to buffer where the data will be written.
 * @param len : Length of the buffer.
 *
 * @return How many bytes are written to the buffer.
 */
extern ACE_size_t ACE_fread (ACE_FILE * in, char *buf, ACE_size_t len);

/**
 * Write to IO stream.
 *
 * @param out : File descriptor.
 * @param buf : Pointer to buffer which will be written.
 * @param len : Length of the buffer.
 */
extern ACE_size_t ACE_fwrite (ACE_FILE * out, char *buf, ACE_size_t len);

/**
 * Get character from IO stream.
 *
 * @param in : File descriptor or NULL.
 *
 * @return Character or EOF.
 */
extern int ACE_fgetc (ACE_FILE * in);

/**
 * Put character to IO stream.
 *
 * @param out : File descriptor or NULL.
 * @param c : Character.
 */
extern int ACE_fputc (ACE_FILE * out, char c);

/**
 * Put sting to IO stream.
 *
 * @param out : File descriptor or NULL.
 * @param string : Pointer to string.
 */
extern int ACE_fputs (ACE_FILE * out, char *string);

/**
 * Formated print to IO stream.
 *
 * @param out : File descriptor or NULL.
 * @param fmt : Format string.
 */
extern int ACE_fprintf (ACE_FILE * out, const char *fmt, ...);



/**
 * Read from standard IO.
 *
 * @param buf : Pointer to buffer into which the data is written.
 * @param len : Length of the buffer.
 *
 * @return How many Bytes are written to the buffer.
 */
extern ACE_size_t ACE_read (char *buf, ACE_size_t len);

/**
 * Write to standard IO.
 *
 * @param buf : Pointer to buffer which will be written.
 * @param len : Length of the buffer.
 */
extern ACE_size_t ACE_write (char *buf, ACE_size_t len);

/**
 * Get character from standard IO.
 *
 * @return Character or EOF.
 */
extern int ACE_getc (void);

/**
 * Put character to standard IO.
 *
 * @param c : Character.
 */
extern int ACE_putc (char c);

/**
 * Put string to standard IO.
 *
 * @param string : Pointer to string.
 */
extern int ACE_puts (char *string);

/**
 * Formated print to standard IO.
 *
 * @param fmt : Format string.
 */
extern int ACE_printf (const char *fmt, ...);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
