/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */  
    
//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// API for configuring and using Timer Counter (TC) peripherals.
///
/// !Usage
/// -# Optionally, use TC_FindMckDivisor() to let the program find the best
///    TCCLKS field value automatically.
/// -# Configure a Timer Counter in the desired mode using TC_Configure().
/// -# Start or stop the timer clock using TC_Start() and TC_Stop().
//------------------------------------------------------------------------------
    
#ifndef DEV_AT91_TC_H
#define DEV_AT91_TC_H
    
#include "dev/arch/at91sam7x/AT91SAM7X256.h"
    
#if !defined(AT91C_ID_TC0) && defined(AT91C_ID_TC012)
#define AT91C_ID_TC0 AT91C_ID_TC012
#endif  /*  */
    
/**
 * Configures a Timer Counter to operate in the given mode. Timer is stopped
 * after configuration and must be restarted with TC_Start(). All the
 * interrupts of the timer are also disabled.
 * @param pTc : Pointer to an AT91S_TC instance.
 * @param mode : Operating mode (TC_CMR value).
 */ 
extern void DEV_at91_TC_configure (AT91S_TC * pTc, unsigned int mode);
 
/**
 * Enables the timer clock and performs a software reset to start the counting.
 * @param pTc : Pointer to an AT91S_TC instance.
 */ 
extern void DEV_at91_TC_start (AT91S_TC * pTc);
 
/**
 * Disables the timer clock, stopping the counting.
 * @param pTc  Pointer to an AT91S_TC instance.
 */ 
extern void DEV_at91_TC_stop (AT91S_TC * pTc);
 
/**
 * Finds the best MCK divisor given the timer frequency and MCK. The result
 * is guaranteed to satisfy the following equation:
 * \pre
 * (MCK / (DIV * 65536)) <= freq <= (MCK / DIV)
 * \endpre
 * with DIV being the highest possible value.
 * @param freq : Desired timer frequency.
 * @param mck : Master clock frequency.
 * @param div : Divisor value.
 * @param tcclks : TCCLKS field value for divisor.
 * @return 1 if a proper divisor has been found; otherwise 0.
 */ 
extern unsigned char DEV_at91_TC_find_mck_divisor (unsigned int freq, unsigned int mck,
                                                   unsigned int *div, unsigned int *tcclks);
 
#endif // DEV_AT91_TC_H
    
