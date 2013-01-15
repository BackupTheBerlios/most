/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_CPU_H
#define ACE_CPU_H

/** @addtogroup ace
 *
 * @{
 */


/** @defgroup ace_cpu cpu.h
 *
 * Cpu architecture definitions.
 * @{
 */

/*------------- Interface ------------------------------------------*/

/** H8300 CPU */
#define ACE_CPU_H8300    1

/** I386 CPU */
#define ACE_CPU_I386     2

/** ARM CPU */
#define ACE_CPU_ARM      3

/** 68000 CPU */
#define ACE_CPU_M68K     4

/*
"Little Endian" means that the low-order byte of the number is stored in memory at the lowest address, and the high-order byte at the highest address. (The little end comes first.) For example, a 4 byte LongInt

    Byte0 Byte1 Byte2 Byte3

will be arranged in memory as follows:

    Base Address+0   Byte3
    Base Address+1   Byte2
    Base Address+2   Byte1
    Base Address+3   Byte0

Intel processors (those used in PC's) use "Little Endian" byte order.

"Big Endian" means that the high-order byte of the number is stored in memory at the lowest address, and the low-order byte at the highest address. (The big end comes first.) Our LongInt, would then be stored as:

    Base Address+0   Byte0
    Base Address+1   Byte1
    Base Address+2   Byte2
    Base Address+3   Byte3

Motorola processors (those used in Mac's) use "Big Endian" byte order.
Network Byte Order is also "Big Endian"

*/

/** Motorola */
#define ACE_BIG_ENDIAN      0123

/** Intel */
#define ACE_LITTLE_ENDIAN   3210



#ifdef  MOST_CPU
#if MOST_CPU == ACE_CPU_H8300
/** We have a H8300 cpu*/
#define ACE_CPU ACE_CPU_H8300
#elif MOST_CPU == ACE_CPU_I386
/** We have a I386 cpu*/
#define ACE_CPU ACE_CPU_I386
#elif MOST_CPU == ACE_CPU_ARM
/** We have an ARM cpu*/
#define ACE_CPU ACE_CPU_ARM
#elif MOST_CPU == ACE_CPU_M68K
/** We have a 68000 cpu*/
#define ACE_CPU ACE_CPU_M68K
#else
#error "MOST_CPU not supported!"
#endif
#else
#error "MOST_CPU not defined!"
#endif


#ifdef MOST_ENDIAN
#if MOST_ENDIAN == ACE_BIG_ENDIAN
/** We have big endian byteorder */
#define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#elif MOST_ENDIAN == ACE_LITTLE_ENDIAN
/** We have little endian byteorder */
#define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#else
#error "MOST_ENDIAN not valid!"
#endif
#else
#error "MOST_ENDIAN not defined!"
#endif



/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
