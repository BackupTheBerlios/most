/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDLIB_H
#define ACE_STDLIB_H

#include "ace/arch/cpu.h"
#include "ace/stddef.h"

#include <uso/heap.h>

/** @defgroup stdlib stdlib.h
 *
 * Standard library functions.
 *
 * @{
 */

/*------------- Interface ------------------------------------------*/

/** ascii (dec) to integer. 
 *  @param s Pointer auf Ziffer
 *  @return Wert
 */
extern int ACE_atoi (const char *s);

/** ascii to long.
 *  @param s Pointer auf Ziffer
 *  @return binary
 */
extern long ACE_atol (const char *s);

/** char to hex.
 *  @param c Character
 *  @return hex value
 */
extern int ACE_atox (char c);

/** 2 char to hex.
 *  @param s Pointer to string
 *  @return hex value
 */
extern int ACE_atoxc (char* s);


#define ACE_ntohl(n) ACE_htonl(n)
#define ACE_ntohs(n) ACE_htons(n)

#ifdef ACE_BYTE_ORDER
#if ACE_BYTE_ORDER == ACE_LITTLE_ENDIAN

/**
 * If BYTE_ORDER = LITTLE_ENDIAN.
 * @param n : short to convert.
 * @return value whith changed byte order.
 */
extern ACE_u16_t ACE_htons (ACE_u16_t n);

/**
 * If BYTE_ORDER = LITTLE_ENDIAN.
 * @param n : long to convert.
 * @return value whith changed byte order.
 */
extern ACE_u32_t ACE_htonl (ACE_u32_t n);

#elif ACE_BYTE_ORDER == ACE_BIG_ENDIAN

/** If BYTE_ORDER = BIG_ENDIAN */
#define ACE_htons(n) (n)

/** If BYTE_ORDER = BIG_ENDIAN */
#define ACE_htonl(n) (n)

#elif
#error "ACE_BYTE_ORDER not valid!"
#endif
#elif
#error "ACE_BYTE_ORDER not defined!"
#endif

extern void ACE_stdlib_init (USO_heap_t* heap);
extern void *ACE_malloc (ACE_size_t size);
extern void ACE_free (void *block);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
