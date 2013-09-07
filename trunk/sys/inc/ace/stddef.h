/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDDEF_H
#define ACE_STDDEF_H

#include <ace/cpu.h>

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

/** long call */
#ifdef ACE_CPU
#if ACE_CPU == ACE_CPU_ARM
#define ACE_LONG_CALL_          __attribute__((__long_call__))
#else
#define ACE_LONG_CALL_
#endif
#else
#error "ACE_CPU not defined!"
#endif

/** Null pointer. */
#define NULL ((void*)0)

/** Calculate elements in an array. */
#define ACE_ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
/** Minimum from (a,b). */
#define ACE_MIN(a,b) ((a) <= (b) ? (a) : (b))
/** Maximum from (a,b). */
#define ACE_MAX(a,b) ((a) >= (b) ? (a) : (b))
/** Maximum from (a,b). */
#define ACE_AMOUNT(a) ((a) >= 0 ? (a) : (-a))

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

/** 64 Bit unsigned. */
typedef unsigned long long ACE_u64_t;
/** 64 Bit signed. */
typedef signed long long ACE_s64_t;


#define ACE_NUL     ((char)0x00)     /**< null */
#define ACE_SOH     ((char)0x01)     /**< start of heading */
#define ACE_STX     ((char)0x02)     /**< start of text */
#define ACE_ETX     ((char)0x03)     /**< end of text */
#define ACE_EOT     ((char)0x04)     /**< end of transmission */
#define ACE_ENQ     ((char)0x05)     /**< enquiry */
#define ACE_ACK     ((char)0x06)     /**< acknowledge */
#define ACE_BELL    ((char)0x07)     /**< bell */
#define ACE_BS      ((char)0x08)     /**< backspace */
#define ACE_TAB     ((char)0x09)     /**< horizontal tab */
#define ACE_LF      ((char)0x0A)     /**< line feed, NL new line */
#define ACE_VT      ((char)0x0B)     /**< vertical tab */
#define ACE_FF      ((char)0x0C)     /**< form feed, NP new page */
#define ACE_CR      ((char)0x0D)     /**< carriage return */
#define ACE_SO      ((char)0x0E)     /**< shift out */
#define ACE_SI      ((char)0x0F)     /**< shift in */
#define ACE_DLE     ((char)0x10)     /**< data link escape */
#define ACE_DC1     ((char)0x11)     /**< device control 1, XON Ausgabe fortsetzen */
#define ACE_DC2     ((char)0x12)     /**< device control 2, Reset */
#define ACE_DC3     ((char)0x13)     /**< device control 3, XOFF Ausgabe anhalten */
#define ACE_DC4     ((char)0x14)     /**< device control 4 */
#define ACE_NAK     ((char)0x15)     /**< negative acknowledge */
#define ACE_SYN     ((char)0x16)     /**< synchronous idle */
#define ACE_ETB     ((char)0x17)     /**< end of trans. block */
#define ACE_CAN     ((char)0x18)     /**< cancel */
#define ACE_EM      ((char)0x19)     /**< end of medium */
#define ACE_SUB     ((char)0x1A)     /**< substitute */
#define ACE_ESC     ((char)0x1B)     /**< escape */
#define ACE_FS      ((char)0x1C)     /**< file separator */
#define ACE_GS      ((char)0x1D)     /**< group separator */
#define ACE_RS      ((char)0x1E)     /**< record separator */
#define ACE_US      ((char)0x1F)     /**< unit separator */
#define ACE_Space   ((char)0x20)     /**< space */
#define ACE_DEL     ((char)0x7F)     /**< delete */

#define ACE_CTRL_A     ACE_SOH       /**< start of heading */
#define ACE_CTRL_B     ACE_STX       /**< start of text */
#define ACE_CTRL_C     ACE_ETX       /**< end of text */
#define ACE_CTRL_D     ACE_EOT       /**< end of transmission */
#define ACE_CTRL_E     ACE_ENQ       /**< enquiry */
#define ACE_CTRL_F     ACE_ACK       /**< acknowledge */
#define ACE_CTRL_G     ACE_BELL      /**< bell */
#define ACE_CTRL_H     ACE_BS        /**< backspace */
#define ACE_CTRL_I     ACE_TAB       /**< horizontal tab */
#define ACE_CTRL_J     ACE_LF        /**< line feed, NL new line */
#define ACE_CTRL_K     ACE_VT        /**< vertical tab */
#define ACE_CTRL_L     ACE_FF        /**< form feed, NP new page */
#define ACE_CTRL_M     ACE_CR        /**< carriage return */
#define ACE_CTRL_N     ACE_SO        /**< shift out */
#define ACE_CTRL_O     ACE_SI        /**< shift in */
#define ACE_CTRL_P     ACE_DLE       /**< data link escape */
#define ACE_CTRL_Q     ACE_DC1       /**< device control 1, XON Ausgabe fortsetzen  */
#define ACE_CTRL_R     ACE_DC2       /**< device control 2, Reset */
#define ACE_CTRL_S     ACE_DC3       /**< device control 3, XOFF Ausgabe anhalten */
#define ACE_CTRL_T     ACE_DC4       /**< device control 4 */
#define ACE_CTRL_U     ACE_NAK       /**< negative acknowledge */
#define ACE_CTRL_V     ACE_SYN       /**< synchronous idle */
#define ACE_CTRL_W     ACE_ETB       /**< end of trans. block */
#define ACE_CTRL_X     ACE_CAN       /**< cancel */
#define ACE_CTRL_Y     ACE_EM        /**< end of medium */
#define ACE_CTRL_Z     ACE_SUB       /**< substitute */


/** @}
 */

/** @}
 */

#endif
