/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STRING_H
#define ACE_STRING_H

#include "ace/stddef.h"

/** @defgroup string string.h
 *
 * Standard string and memmory functions.
 *
 * @{
 */

/*------------- Interface ------------------------------------------*/

/** How you like */
#define bcopy(s, d, l) memcpy(d, s, l)

/** More special */
#define bzero(d, l)    memset(d, 0, l)


/** Zeichenkette ct in Vektor s kopieren, inklusive '\\0'.
 *  @return s
 */
extern char *strcpy (char *s, const char *ct);

/** ho"chstens n Zeichen aus ct in s kopieren.
 *  Mit '\\0' auffu"llen, wenn ct weniger als n Zeichen hat.
 *  @return s 
 */
extern char *strncpy (char *s, const char *ct, size_t n);

/** Zeichenkette ct hinten an die Zeichenkette s anfu"gen.
 *  @return s
 */
extern char *strcat (char *s, const char *ct);

/** ho"chstens n Zeichen von ct hinten an die Zeichenkette 
 *  s anfu"gen und s mit '\\0' abschliesen.
 *  @return s
 */
extern char *strncat (char *s, const char *ct, size_t n);

/** Zeichenkette cs und ct vergleichen.
 *  @return <0 wenn cs < ct, 0 wenn cs == ct, >0 wenn cs>ct
 */
extern int strcmp (const char *cs, const char *ct);

/** ho"chstens n Zeichen von cs mit der Zeichenkette ct vergleichen.
 *  @return <0 wenn cs < ct, 0 wenn cs == ct, >0 wenn cs>ct
 */
extern int strncmp (const char *cs, const char *ct, size_t n);

/** liefert Zeiger auf das erste c in cs oder NULL, falls nicht vorhanden.
 */
extern char *strchr (const char *cs, int c);

/** liefert Zeiger auf das letzte c in cs oder NULL, falls nicht vorhanden.
 */
extern char *strrchr (const char *cs, int c);

/** liefert Anzahl der Zeichen am Anfang von cs, die sa"mtlich in ct vorkommen.
 */
extern size_t strspn (const char *cs, const char *ct);

/** liefert Zeiger auf die Position in cs, an der irgendein Zeichen
 *  aus ct erstmals vorkommt,
 *  oder NULL , falls keines vorkommt.
 */
extern char *strpbrk (const char *cs, const char *ct);

/** liefert Zeiger auf erste Kopie von ct in cs oder NULL, falls nicht vorhanden.
 */
extern char *strstr (const char *cs, const char *ct);

/** liefert La"nge von cs (ohne '\\0')
 */
extern size_t strlen (const char *cs);

/** durchsucht s nach Zeichnfolgen,
 *  die durch Zeichen aus ct begrenzt sind. siehe K/R B.4.
 */
extern char *strtok (char *s, const char *ct);

/** kopiert n Zeichen von ct nach s.
 *  @return s
 */
extern void *memcpy (void *s, const void *ct, size_t n);

/** wie memcpy, funktioniert aber auch wenn die Objekte u"berlappen.
 */
extern void *memmove (void *s, const void *ct, size_t n);

/** vergleicht die ersten n Zeichen von cs mit ct.
 *  @return <0 wenn cs < ct, 0 wenn cs == ct, >0 wenn cs>ct
 */
extern int memcmp (const void *cs, const void *ct, size_t n);

/** setzt die ersten n Zeichen von s auf den Wert c.
 *  @return s
 */
extern void *memset (void *s, int c, size_t n);


/** liefert La"nge von cs (ohne '\\0')
 *  wenn cs < n sonst n.
 */
extern size_t strnlen (const char *cs, size_t n);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
