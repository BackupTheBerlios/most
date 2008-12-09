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
 * memmory functions are used by the compiler (gcc) -> no ACE prefix
 * @{
 */

/*------------- Interface ------------------------------------------*/

/** How you like */
#define ACE_bcopy(s, d, l) memcpy(d, s, l)

/** More special */
#define ACE_bzero(d, l)    memset(d, 0, l)


/** Zeichenkette ct in Vektor s kopieren, inklusive '\\0'.
 *  @return s
 */
extern char *ACE_strcpy (char *s, const char *ct);

/** ho"chstens n Zeichen aus ct in s kopieren.
 *  Mit '\\0' auffu"llen, wenn ct weniger als n Zeichen hat.
 *  @return s 
 */
extern char *ACE_strncpy (char *s, const char *ct, ACE_size_t n);

/** Zeichenkette ct hinten an die Zeichenkette s anfu"gen.
 *  @return s
 */
extern char *ACE_strcat (char *s, const char *ct);

/** ho"chstens n Zeichen von ct hinten an die Zeichenkette 
 *  s anfu"gen und s mit '\\0' abschliesen.
 *  @return s
 */
extern char *ACE_strncat (char *s, const char *ct, ACE_size_t n);

/** Zeichenkette cs und ct vergleichen.
 *  @return <0 wenn cs < ct, 0 wenn cs == ct, >0 wenn cs>ct
 */
extern int ACE_strcmp (const char *cs, const char *ct);

/** ho"chstens n Zeichen von cs mit der Zeichenkette ct vergleichen.
 *  @return <0 wenn cs < ct, 0 wenn cs == ct, >0 wenn cs>ct
 */
extern int ACE_strncmp (const char *cs, const char *ct, ACE_size_t n);

/** liefert Zeiger auf das erste c in cs oder NULL, falls nicht vorhanden.
 */
extern char *ACE_strchr (const char *cs, int c);

/** liefert Zeiger auf das letzte c in cs oder NULL, falls nicht vorhanden.
 */
extern char *ACE_strrchr (const char *cs, int c);

/** liefert Anzahl der Zeichen am Anfang von cs, die sa"mtlich in ct vorkommen.
 */
extern ACE_size_t ACE_strspn (const char *cs, const char *ct);

/** liefert Zeiger auf die Position in cs, an der irgendein Zeichen
 *  aus ct erstmals vorkommt,
 *  oder NULL , falls keines vorkommt.
 */
extern char *ACE_strpbrk (const char *cs, const char *ct);

/** liefert Zeiger auf erste Kopie von ct in cs oder NULL, falls nicht vorhanden.
 */
extern char *ACE_strstr (const char *cs, const char *ct);

/** liefert La"nge von cs (ohne '\\0')
 */
extern ACE_size_t ACE_strlen (const char *cs);

/** durchsucht s nach Zeichnfolgen,
 *  die durch Zeichen aus ct begrenzt sind. siehe K/R B.4.
 */
extern char *ACE_strtok (char *s, const char *ct);

/** kopiert n Zeichen von ct nach s.
 *  @return s
 */
extern void *memcpy (void *s, const void *ct, ACE_size_t n);

/** wie memcpy, funktioniert aber auch wenn die Objekte u"berlappen.
 */
extern void *memmove (void *s, const void *ct, ACE_size_t n);

/** vergleicht die ersten n Zeichen von cs mit ct.
 *  @return <0 wenn cs < ct, 0 wenn cs == ct, >0 wenn cs>ct
 */
extern int memcmp (const void *cs, const void *ct, ACE_size_t n);

/** setzt die ersten n Zeichen von s auf den Wert c.
 *  @return s
 */
extern void *memset (void *s, int c, ACE_size_t n);


/** liefert La"nge von cs (ohne '\\0')
 *  wenn cs < n sonst n.
 */
extern ACE_size_t ACE_strnlen (const char *cs, ACE_size_t n);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
