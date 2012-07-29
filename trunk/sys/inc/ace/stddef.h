/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDDEF_H
#define ACE_STDDEF_H

/** @addtogroup ace
 *
 * @{
 */

/** @defgroup stddef stddef.h
 *
 * Standard definitions.
 * NULL, TRUE, FALSE without ACE prefix for syntax highlighting.
 * @{
 */

/*------------- Definitions ------------------------------------------*/

/** Define a function inline. */
#define ACE_INLINE_          __inline__
/** Pack structures. */
#define ACE_PACKED_          __attribute__ ((packed))
/** Define a section.*/
#define ACE_SECTION_(name)   __attribute__ ((section (name)))
/** Function is an interrupt handler.*/
#define ACE_INTERRUPT_       __attribute__ ((interrupt_handler))
/** Set alignment.*/
#define ACE_ALIGNED_(alignment) __attribute__ ((aligned (alignment)))

/** Null pointer. */
#define NULL ((void*)0)

/** Calculate elements in an array. */
#define ACE_ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
/** Minimum from (a,b). */
#define ACE_MIN(a,b) ((a) <= (b) ? (a) : (b))
/** Maximum from (a,b). */
#define ACE_MAX(a,b) ((a) >= (b) ? (a) : (b))

/** Boolean enumeration. */
enum ACE_bool
{
    FALSE = 0,     /**< FALSE = 0*/
    TRUE = 1      /**< TRUE = 1*/
};

/** Boolean type. */
typedef enum ACE_bool ACE_bool_t;

/** Type which sizeof operator returns. */
typedef unsigned long ACE_size_t;

/** Signed size type. */
typedef long ACE_ssize_t;

/** 8 Bit unsigned. */
typedef unsigned char ACE_u8_t;
/** 8 Bit signed. */
typedef signed char ACE_s8_t;

/** 16 Bit unsigned. */
typedef unsigned short ACE_u16_t;
/** 16 Bit signed. */
typedef signed short ACE_s16_t;

/** 32 Bit unsigned. */
typedef unsigned long ACE_u32_t;
/** 32 Bit signed. */
typedef signed long ACE_s32_t;

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
