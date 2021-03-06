/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDLIB_H
#define ACE_STDLIB_H

#include <ace/stddef.h>

#include <uso/heap.h>
#include <uso/mutex.h>

/** @addtogroup ace
 *
 * @{
 */

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
extern int ACE_xtoi (char c);

/** Convert 2 char representing a hexadecimal value to an integer value.
 *  @param s Pointer to string.
 *  @return Integer value.
 */
extern int ACE_xxtoi (char *s);

extern long ACE_strtol(const char *nptr, char **endptr, int base);
extern unsigned long ACE_strtoul(const char *ptr, char **endptr, int base);


/** Conversion of data from or to network order is the same.*/
#define ACE_ntohl(n) ACE_htonl(n)
/** Conversion of data from or to network order is the same.*/
#define ACE_ntohs(n) ACE_htons(n)

#if defined MOST_ENDIAN_LITTLE

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

#elif defined MOST_ENDIAN_BIG

/** If BYTE_ORDER = BIG_ENDIAN no conversion is done. */
#define ACE_htons(n) (n)

/** If BYTE_ORDER = BIG_ENDIAN no conversion is done. */
#define ACE_htonl(n) (n)

#else
#error "Byte_order not defined!"
#endif

/**
 * This function must be called before you can use the stdlib functions.
 * @param heap : Pointer to a heap which will be used with the malloc and free functions.
 * @param abort_handler: function which will be called in an abort.
 * @param panic_handler: function which will be called for a panic.
 */
extern void ACE_stdlib_init (USO_heap_t * heap,
                             void (*abort_handler) (char *msg, char *file, int line),
                             void (*panic_handler) (char *msg, char *file, int line));

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

#define ACE_ABORT(msg) ACE_abort(msg, __FILE__, __LINE__)

/**
 * Abort program execution. Not correctable error
 * but you should be able to print the error message.
 * @param msg : error message.
 * @param file : filename from where the function is called.
 * @param line : line number from where the function is called.
 *
 * This function will not return.
 * If you have registered an abort handler, this handler is called.
 */
extern void ACE_abort (char *msg, char *file, int line);

#define ACE_PANIC(msg) ACE_panic(msg, __FILE__, __LINE__)

/**
 * Abort program execution immediate, system is corrupted and
 * further programm execution is not possible.
 * @param msg : error message.
 * @param file : filename from where the function is called.
 * @param line : line number from where the function is called.
 *
 * This function will not return.
 * If you have registered an panic handler, this handler is called.
 */
extern void ACE_panic (char *msg, char *file, int line);

extern USO_mutex_t ACE_lock;

extern void ACE_seed(ACE_u32_t seed);
extern ACE_u32_t ACE_rand(void);


/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
