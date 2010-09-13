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

/** Convert a string representing a decimal number to an integer value.
 *
 *  @param s : Pointer to string.
 *  @return Integer value.
 */
extern int ACE_atoi (const char *s);

/** Convert a string representing a decimal number to a long integer value.
 *  @param s Pointer to string.
 *  @return Long integer value.
 */
extern long ACE_atol (const char *s);

/** Convert a char representing a hexadecimal digit to an integer value.
 *  @param c Character representing a hexadecimal digit.
 *  @return Integer value.
 */
extern int ACE_atox (char c);

/** Convert 2 char representing a hexadecimal value to an integer value.
 *  @param s Pointer to string.
 *  @return Integer value.
 */
extern int ACE_atoxc (char* s);

/** Conversion of data from or to network order is the same.*/
#define ACE_ntohl(n) ACE_htonl(n)
/** Conversion of data from or to network order is the same.*/
#define ACE_ntohs(n) ACE_htons(n)

#ifdef ACE_BYTE_ORDER
#if ACE_BYTE_ORDER == ACE_LITTLE_ENDIAN

/**
 * If BYTE_ORDER = LITTLE_ENDIAN do a conversion..
 * @param n : Short data to convert.
 * @return Value with network byte order.
 */
extern ACE_u16_t ACE_htons (ACE_u16_t n);

/**
 * If BYTE_ORDER = LITTLE_ENDIAN do a conversion.
 * @param n : Long data to convert.
 * @return Value with network byte order.
 */
extern ACE_u32_t ACE_htonl (ACE_u32_t n);

#elif ACE_BYTE_ORDER == ACE_BIG_ENDIAN

/** If BYTE_ORDER = BIG_ENDIAN no conversion is done. */
#define ACE_htons(n) (n)

/** If BYTE_ORDER = BIG_ENDIAN no conversion is done. */
#define ACE_htonl(n) (n)

#else
#error "ACE_BYTE_ORDER not valid!"
#endif
#else
#error "ACE_BYTE_ORDER not defined!"
#endif

/**
 * This function must be called before you can use the stdlib functions.
 * @param heap : Pointer to a heap which will be used with the malloc and free functions.
 */
extern void ACE_stdlib_init (USO_heap_t* heap);

/**
 * Allocate memory.
 * @param size : Size of the memory which should be allocated.
 * @return Pointer to allocated memory or NULL if the memory is not available.
 */
extern void *ACE_malloc (ACE_size_t size);

/**
 * Free memory.
 * @param block : Allocated memory which shall be given back to the heap.
 */
extern void ACE_free (void *block);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
