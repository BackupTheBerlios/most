/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_CPU_H
#define ACE_CPU_H

/** @defgroup cpu cpu.h
 *
 * Cpu architecture definitions.
 * @{
 */

/*------------- Interface ------------------------------------------*/

/** I386 CPU */
#define ACE_CPU_I386     1

/** H8300 Microcontroller */
#define ACE_CPU_H8300    2

/** ARM Microcontroller */
#define ACE_CPU_ARM      3

/*
"Little Endian" means that the low-order byte of the number is stored in memory at the lowest address, and the high-order byte at the highest address. (The little end comes first.) For example, a 4 byte LongInt

    Byte3 Byte2 Byte1 Byte0

will be arranged in memory as follows:

    Base Address+0   Byte0
    Base Address+1   Byte1
    Base Address+2   Byte2
    Base Address+3   Byte3

Intel processors (those used in PC's) use "Little Endian" byte order.

"Big Endian" means that the high-order byte of the number is stored in memory at the lowest address, and the low-order byte at the highest address. (The big end comes first.) Our LongInt, would then be stored as:

    Base Address+0   Byte3
    Base Address+1   Byte2
    Base Address+2   Byte1
    Base Address+3   Byte0

Motorola processors (those used in Mac's) use "Big Endian" byte order.
Network Byte Order is also "Big Endian"

*/

/** Motorola */
#define ACE_BIG_ENDIAN    3210

/** Intel */
#define ACE_LITTLE_ENDIAN 0123


#ifdef  MOST_TARGET_I386

/** We have a I386 cpu*/
#define ACE_CPU ACE_CPU_I386

/** We have little endian byteorder */
#define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN

#endif


#ifdef MOST_TARGET_H8300

/** We have a H8300 Microcontroller */
#define ACE_CPU ACE_CPU_H8300

/** We have big endian byteorder */
#define ACE_BYTE_ORDER ACE_BIG_ENDIAN

#endif

#ifdef MOST_TARGET_ARM

/** We have an ARM Microcontroller */
#define ACE_CPU ACE_CPU_ARM

/** We have big endian byteorder */
#define ACE_BYTE_ORDER ACE_BIG_ENDIAN

#endif

#define ACE_TICKS_PER_SEC 1000L

#define ACE_MSEC_2_TICKS(ACE_msec) ( (ACE_TICKS_PER_SEC * (ACE_msec)) / 1000L  )
#define ACE_TICKS_2_MSEC(ACE_ticks) ( (1000L * (ACE_ticks)) / ACE_TICKS_PER_SEC )

#define ACE_LOOPS_PER_MSEC 765L

#define ACE_USEC_2_LOOPS(ACE_usec) ( (ACE_LOOPS_PER_MSEC * (ACE_usec)) / 1000L  )
 
/*------------------------------------------------------------------------*/

/** @}
 */

#endif
