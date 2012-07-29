/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_H8_BITOPS_H
#define DEV_H8_BITOPS_H

#include "dev/arch/h8/bits.h"

/** @addtogroup dev
 *
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

/** @defgroup bitops bitops.h
 *
 * @{
 */


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

/** @}
 */

#endif
