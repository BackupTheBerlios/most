/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDDEF_H
#define ACE_STDDEF_H

/** @defgroup stddef stddef.h
 *
 * Standard definitions.
 *
 * @{
 */

/*------------- Definitions ------------------------------------------*/

#define _INLINE_          __inline__
#define _PACKED_          __attribute__ ((packed))
#define _SECTION_(name)   __attribute__ ((section (name)))
#define _INTERRUPT_       __attribute__ ((interrupt_handler))

/** NULL pointer */
#define NULL ((void*)0)

/** Calculate elements in an array. */
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#define MIN(a,b) ((a) <= (b) ? (a) : (b))
#define MAX(a,b) ((a) >= (b) ? (a) : (b))

/** Boolean enumeration */
enum bool
{ 
	FALSE,
  	TRUE
};

/** Boolean type */
typedef enum bool bool_t;

/** type which sizeof operator returns */
typedef unsigned long size_t;

/** signed size type */
typedef long ssize_t;

/** 8 Bit unsigned */
typedef unsigned char u8_t;
/** 8 Bit signed */
typedef signed char s8_t;

/** 16 Bit unsigned */
typedef unsigned short u16_t;
/** 16 Bit signed */
typedef signed short s16_t;

/** 32 Bit unsigned */
typedef unsigned long u32_t;
/** 32 Bit signed */
typedef signed long s32_t;

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
