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
 * Memory functions are used by the compiler (gcc) for this they have no ACE_ prefix.
 * @{
 */

/*------------- Interface ------------------------------------------*/

/** Block copy does the same as memory copy. */
#define ACE_bcopy(s, d, l) memcpy(d, s, l)

/** Be zero does the same as memory set with a 0 value. */
#define ACE_bzero(d, l)    memset(d, 0, l)


/** Copy \\0 terminated string ct to s, inclusive \\0.
 *  @return s.
 */
extern char *ACE_strcpy (char *s, const char *ct);

/** Copy exactly n characters from ct to s.
 *  Fill up with '\\0' if ct is shorter than n.
 *  @return s.
 */
extern char *ACE_strncpy (char *s, const char *ct, ACE_size_t n);

/** Concatenate ct to the end of s. Both memory blocks have to be \\0 terminated.
 *  @return s.
 */
extern char *ACE_strcat (char *s, const char *ct);

/** Concatenate n characters of ct to the end of s.
 * Both memory blocks have to be \\0 terminated.
 *  @return s.
 */
extern char *ACE_strncat (char *s, const char *ct, ACE_size_t n);

/** Compare string cs with ct.
 *  @return <0 if (cs < ct), 0 if (cs == ct) and  >0 if (cs>ct).
 */
extern int ACE_strcmp (const char *cs, const char *ct);

/** Compare n characters of cs with ct.
 *  @return <0 if (cs < ct), 0 if (cs == ct) or >0 if (cs > ct).
 */
extern int ACE_strncmp (const char *cs, const char *ct, ACE_size_t n);

/** Get a pointer to the first character c found in string cs or NULL if c is not found.
 */
extern char *ACE_strchr (const char *cs, int c);

/** Get a pointer to the last character c found in string cs or NULL if c is not found.
 */
extern char *ACE_strrchr (const char *cs, int c);

/** Get the length of the initial segment of cs which consists entierly of characters in ct.
 */
extern ACE_size_t ACE_strspn (const char *cs, const char *ct);

/** Get pointer of position in cs where any character of ct will occour the first time,
 * or NULL if no character of ct is in cs.
 */
extern char *ACE_strpbrk (const char *cs, const char *ct);

/** Get pointer to first string ct found in cs or NULL if string ct is not found in cs.
 */
extern char *ACE_strstr (const char *cs, const char *ct);

/** Get length of string cs without terminating \0.
 */
extern ACE_size_t ACE_strlen (const char *cs);

/** A sequence of calls to strtok() breaks the string pointed to by s into a sequence of tokens,
 *  each of which is delimited by a byte from the string pointed to by ct.
 */
extern char *ACE_strtok (char *s, const char *ct);

/** Copy n characters from ct to s.
 *  @return s.
 */
extern void *memcpy (void *s, const void *ct, ACE_size_t n);

/** As memcpy, but works also if the objects overlaps.
 */
extern void *memmove (void *s, const void *ct, ACE_size_t n);

/** Compare the first n characters of cs with ct.
 *  @return <0 if (cs < ct), 0 if (cs == ct) and >0 if (cs > ct).
 */
extern int memcmp (const void *cs, const void *ct, ACE_size_t n);

/** Initialize the first n characters of s to the value of n.
 *  @return s.
 */
extern void *memset (void *s, int c, ACE_size_t n);


/** Get the length of the string cs without terminating \\0,
 *  if cs has less then n characters else return n.
 */
extern ACE_size_t ACE_strnlen (const char *cs, ACE_size_t n);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
