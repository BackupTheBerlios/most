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
#define EOF (-1)

typedef MFS_stream_t FILE;

/*------------------------------------------------------------------------*/

/*---------------- Interface ---------------------------------------------*/

/**
 *  sprintf funktioniert wie printf, nur wird die Ausgabe in den Zeichenvektor
 *  s geschrieben und mit '\\0' abgeschlossen.
 *  @param buf (Zeichenvektor s) mu gro genug fuer das Resultat sein
 *  @param fmt Format Zeichenkette
 *  @return Anzahl der geschriebenen Zeichen, negativ wenn Fehler
 */
extern int sprintf (char *buf, const char *fmt, ...);

/** vsprintf quivalent zu sprintf, jedoch wird die variable Argumentliste
 *  duch arg ersetzt.
 *  Dieser Wert wird mit dem Makro va_start und vielleicht mit Aufrufen
 *  von va_arg initialisiert.
 *  Siehe dazu Beschreibung von <stdarg.h> K/R B.7
 */
extern int vsprintf (char *buf, const char *fmt, va_list_t args);


 
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
extern size_t fread (FILE * in, char *buf, size_t len);

/**
 * Write to IO stream.
 *
 * @param out : File descriptor.
 * @param buf : Pointer to buffer which will be written.
 * @param len : Length of the buffer.
 */
extern size_t fwrite (FILE * out, char *buf, size_t len);

/**
 * Get character from IO stream.
 *
 * @param in : Input Stream or NULL.
 *
 * @return Character or EOF.
 */
extern int fgetc (FILE * in);

/**
 * Put character to IO stream.
 *
 * @param out : Output Stream or NULL.
 * @param c : Character.
 */
extern int fputc (FILE * out, char c);

/**
 * Put sting to IO stream.
 *
 * @param out : Output Stream or NULL.
 * @param string : Pointer to String.
 */
extern int fputs (FILE * out, char *string);

/**
 * Formated print to IO stream.
 *
 * @param out : Output Stream or NULL.
 * @param fmt : Format string.
 */
extern int fprintf (FILE * out, const char *fmt, ...);



/**
 * Read from standard IO.
 *
 * @param buf : Pointer to buffer into which will be readed.
 * @param len : Length of the buffer.
 *
 * @return How many Bytes are written to the buffer.
 */
extern size_t read (char *buf, size_t len);

/**
 * Write to standard IO.
 *
 * @param buf : Pointer to buffer which will be written.
 * @param len : Length of the buffer.
 */
extern size_t write (char *buf, size_t len);

/**
 * Get character from standard IO.
 *
 * @return Character or EOF.
 */
extern int getc (void);

/**
 * Put character to standard IO.
 *
 * @param c : Character.
 */
extern int putc (char c);

/**
 * Put string to standard IO.
 *
 * @param string : Pointer to String.
 */
extern int puts (char *string);

/**
 * Formated print to standard IO.
 *
 * @param fmt : Format string.
 */
extern int printf (const char *fmt, ...);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
