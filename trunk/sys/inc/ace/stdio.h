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

/** End of file */
#define ACE_EOF (-1)

typedef MFS_stream_t ACE_FILE;

/*------------------------------------------------------------------------*/

/*---------------- Interface ---------------------------------------------*/

/**
 *  sprintf funktioniert wie printf, nur wird die Ausgabe in den Zeichenvektor
 *  s geschrieben und mit '\\0' abgeschlossen.
 *  @param buf (Zeichenvektor s) mu gro genug fuer das Resultat sein
 *  @param fmt Format Zeichenkette
 *  @return Anzahl der geschriebenen Zeichen, negativ wenn Fehler
 */
extern int ACE_sprintf (char *buf, const char *fmt, ...);

/** vsprintf quivalent zu sprintf, jedoch wird die variable Argumentliste
 *  duch arg ersetzt.
 *  Dieser Wert wird mit dem Makro va_start und vielleicht mit Aufrufen
 *  von va_arg initialisiert.
 *  Siehe dazu Beschreibung von <stdarg.h> K/R B.7
 */
extern int ACE_vsprintf (char *buf, const char *fmt, ACE_va_list_t args);


 
/**
 * Initalize list and buffers.
 */
extern void ACE_stdio_init (void);

/**
 * Read from IO stream.
 *
 * @param in : File descriptor.
 * @param buf : Pointer to buffer into which will be readed.
 * @param len : Length of the buffer.
 *
 * @return How many Bytes are written to the buffer.
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
 * @param in : Input Stream or NULL.
 *
 * @return Character or EOF.
 */
extern int ACE_fgetc (ACE_FILE * in);

/**
 * Put character to IO stream.
 *
 * @param out : Output Stream or NULL.
 * @param c : Character.
 */
extern int ACE_fputc (ACE_FILE * out, char c);

/**
 * Put sting to IO stream.
 *
 * @param out : Output Stream or NULL.
 * @param string : Pointer to String.
 */
extern int ACE_fputs (ACE_FILE * out, char *string);

/**
 * Formated print to IO stream.
 *
 * @param out : Output Stream or NULL.
 * @param fmt : Format string.
 */
extern int ACE_fprintf (ACE_FILE * out, const char *fmt, ...);



/**
 * Read from standard IO.
 *
 * @param buf : Pointer to buffer into which will be readed.
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
 * @param string : Pointer to String.
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
