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

#ifndef DEV_AT91_RST_H
#define DEV_AT91_RST_H

#include <dev/arch/at91/AT91SAM7X256.h>

/**
 * Configure the mode of the RSTC peripheral.
 * The configuration is computed by the lib (AT91C_RSTC_*).
 * @param rstc : Pointer to an RSTC peripheral.
 * @param rmr : Desired mode configuration.
 */
extern void DEV_at91_RST_configure_mode (AT91PS_RSTC rstc, unsigned int rmr);

/**
 * Enable/Disable the detection of a low level on the pin NRST as User Reset
 * @param rstc : Pointer to an RSTC peripheral.
 * @param enable : 1 to enable & 0 to disable.
 */
extern void DEV_at91_RST_set_user_reset_enable (AT91PS_RSTC rstc, unsigned char enable);

/**
 * Enable/Disable the interrupt of a User Reset (USRTS bit in RSTC_RST).
 * @param rstc :  Pointer to an RSTC peripheral.
 * @param enable : 1 to enable & 0 to disable.
 */
extern void DEV_at91_RST_set_user_reset_IT_enable (AT91PS_RSTC rstc, unsigned char enable);

/**
 * Setup the external reset length. The length is asserted during a time of
 * pow(2, powl+1) Slow Clock(32KHz). The duration is between 60us and 2s.
 * @param rstc : Pointer to an RSTC peripheral.
 * @param powl : Power length defined.
 */
extern void DEV_at91_RST_set_ext_reset_length (AT91PS_RSTC rstc, unsigned char powl);

/**
 * Resets the processor.
 * @param rstc  Pointer to an RSTC peripheral.
 */
extern void DEV_at91_RST_processor_reset (AT91PS_RSTC rstc);

/**
 * Resets the peripherals.
 * @param rstc  Pointer to an RSTC peripheral.
 */
extern void DEV_at91_RST_peripheral_reset (AT91PS_RSTC rstc);

/**
 * Asserts the NRST pin for external resets.
 * @param rstc  Pointer to an RSTC peripheral.
 */
extern void DEV_at91_RST_ext_reset (AT91PS_RSTC rstc);

/**
 * Return NRST pin level ( 1 or 0 ).
 * @param rstc  Pointer to an RSTC peripheral.
 */
extern unsigned char DEV_at91_RST_get_NRST_level (AT91PS_RSTC rstc);

/**
 * Returns 1 if at least one high-to-low transition of NRST (User Reset) has
 * been detected since the last read of RSTC_RSR.
 * @param rstc  Pointer to an RSTC peripheral.
 */
extern unsigned char DEV_at91_RST_is_user_reset_detected (AT91PS_RSTC rstc);

/**
 * Return 1 if a software reset command is being performed by the reset
 * controller. The reset controller is busy.
 * @param rstc : Pointer to an RSTC peripheral.
 */
extern unsigned char DEV_at91_RST_is_busy (AT91PS_RSTC rstc);

unsigned int DEV_at91_RST_status (AT91PS_RSTC rstc);


#endif // DEV_AT91_RST_H
