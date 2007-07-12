/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup h8300
 *
 * @{
 */

/** @defgroup bitops bitops.h
 *
 * @{
 */

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
// @}

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
// @}

/**
 * These macros changing the status of one bit using as input the bitmask.
 * Macros of this group have non abbreviated names and starting with BIT...
 * If the bit(mask) value is a constant(defined macro) the preprocessor will 
 * calculate the bitnumber and there will be no additional command added.
 * @{
 */

/** 
 * Clears a bit in the given register or memory operand to zero. 
 * 
 * Makes the same as the asm BCLR command, but takes the bitmask instead of
 * the bitnumber. That�s why it is named different. Only use with 8 bit register
 * or memory operand.
 * 
 * @param bitmask bitmask of bit to clear
 * @param where   register or memory operand of bit to clear
 */
#define H8_BITCLEAR(bitmask,where)    __asm__ volatile ("bclr %Z1,%Y0:8" : "=mr" (where) : "Uri" (DEV_BITNUM8(bitmask)))

/**
 * \brief Sets a bit in the given register or memory operand to one. 
 * 
 * Makes the same as the asm BSET command, but takes the bitmask instead of the
 * bitnumber. That�s why it is named different. Only use with 8 bit register or 
 * memory operand.
 * 
 * @param bitmask bitmask of bit to set
 * @param where   register or memory operand of bit to set
 */
#define H8_BITSET(bitmask,where)      __asm__ volatile ("bset %Z1,%Y0:8" : "=mr" (where) : "Uri" (DEV_BITNUM8(bitmask)))

/**
 * \brief Toggles a bit in the given register or memory operand. 
 * 
 * Makes the same as the asm BNOT command, but takes the bitmask instead of the
 * bitnumber. That�s why it is named different. Only use with 8 bit register or 
 * memory operand.
 * 
 * @param bitmask bitmask of bit to toggle
 * @param where   register or memory operand of bit to toggle
 */
#define H8_BITNOT(bitmask,where)   __asm__ volatile ("bnot %Z1,%Y0:8" : "=mr" (where) : "Uri" (DEV_BITNUM8(bitmask)): "cc")
// @}

/** @}
 */

/** @}
 */

/** @}
 */
