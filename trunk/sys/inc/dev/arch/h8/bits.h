#ifndef DEV_BITS_H
#define DEV_BITS_H

/** @addtogroup dev
 * @{
 */

/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup h8
 *
 * @{
 */

/** @defgroup bits bits.h
 *
 * @{
 */

#define BIT(x)    (1  << (x))   /**< H8 : only use for 16 bit integer. */

#define   BIT0        0x00000001L       /* 1 << 0 */
#define   BIT1        0x00000002L       /* 1 << 1 */
#define   BIT2        0x00000004L       /* 1 << 2 */
#define   BIT3        0x00000008L
#define   BIT4        0x00000010L
#define   BIT5        0x00000020L
#define   BIT6        0x00000040L
#define   BIT7        0x00000080L
#define   BIT8        0x00000100L
#define   BIT9        0x00000200L
#define   BIT10       0x00000400L
#define   BIT11       0x00000800L
#define   BIT12       0x00001000L
#define   BIT13       0x00002000L
#define   BIT14       0x00004000L
#define   BIT15       0x00008000L
#define   BIT16       0x00010000L
#define   BIT17       0x00020000L
#define   BIT18       0x00040000L
#define   BIT19       0x00080000L
#define   BIT20       0x00100000L
#define   BIT21       0x00200000L
#define   BIT22       0x00400000L
#define   BIT23       0x00800000L
#define   BIT24       0x01000000L
#define   BIT25       0x02000000L
#define   BIT26       0x04000000L
#define   BIT27       0x08000000L
#define   BIT28       0x10000000L
#define   BIT29       0x20000000L
#define   BIT30       0x40000000L
#define   BIT31       0x80000000L       /* 1 << 31 */

/**
 * The TESTYBITS() macros test the Y bits left from the base.
 *
 * The macros are done to shorten the definition of the BITNUM macros and if there is a
 * bit set the number of it is given back.
 *
 * @param x    Bitmask with only one bit set.
 * @param base Offset from left where the bit test starts.
 *
 * @return Bitnumber started at zero
 * @{
 */
#define DEV_TEST2BITS(x, base)      (!!((x)>>(base)) +  !!((x)>>((base)+1)))
#define DEV_TEST4BITS(x, base)      (DEV_TEST2BITS(x,base) + DEV_TEST2BITS(x,base+2))
#define DEV_TEST8BITS(x, base)      (DEV_TEST4BITS(x,base) + DEV_TEST4BITS(x,base+4))
/* @} */

/**
 * The BITNUMY() macros converting a bitmask to the bitnumber.
 *
 * The macros give back the number of the bit which is set in a bitmask.
 * If more than one bit is set in the mask you will can a not useful value and
 * at least one bit have to be set..
 * The macros are done for 8, 16 and 32 bit values.
 * When using this macro, always cast its parameter to a
 * proper unsigned data type!
 * If you convert a constant value like a define for a register bit, using
 * this macro the bitnumber is calulated already from the preprocessor.
 *
 * @param x  Bitmask with only one bit set.
 *
 * @return Bitnumber started at zero
 * @{
 */
#define DEV_BITNUM8(x)              (!!((x)>>1) + DEV_TEST2BITS(x,2) + DEV_TEST4BITS(x,4))
#define DEV_BITNUM16(x)             (DEV_BITNUM8(x) + DEV_TEST8BITS(x,8))
#define DEV_BITNUM32(x)             (DEV_BITNUM16(x) + DEV_TEST8BITS(x,16) + DEV_TEST8BITS(x,24))
/* @} */


/** @}
 */

/** @}
 */

/** @}
 */

/** @}
 */

#endif
