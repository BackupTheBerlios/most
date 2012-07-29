/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2007, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
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
/// !Purpose
///
/// Configuration and handling of PIT.
///
/// !Usage
///
/// -# Initialize System timer for a period in �second with
///    PIT_Init
/// -# Set the PIT Periodic Interval Value with PIT_SetPIV
/// -# Enable the PIT with PIT_Enable
/// -# Enable & disable PIT interrupts using PIT_EnableInt and
///    PIT_DisableInt
/// -# Read PIT mode register
///    PIT_GetMode
/// -# Read PIT status register
///    PIT_GetStatus
/// -# Read PIT CPIV and PICNT without ressetting the counters
///    PIT_GetPIIR
/// -# Read System timer CPIV and PICNT without ressetting the counters
///    PIT_GetPIVR
//------------------------------------------------------------------------------

#ifndef DEV_AT91_PIT_H
#define DEV_AT91_PIT_H

#define DEV_AT91_PIT_SEC_IN_USEC 1000000

/** Initialize the System timer.
 *  @param period : Amount of periods per second.
 *  @param pit_frequency : System clock frequency in MHz.
 */
extern void DEV_at91_PIT_init (unsigned int period, unsigned int pit_frequency);

/**
 * Enable PIT periodic interrupt
 */
extern void DEV_at91_PIT_enable_IT (void);

/**
 * Disable PIT periodic interrupt
 */
extern void DEV_at91_PIT_disable_IT (void);

/**
 * Read PIT mode register
 * @return PIT_MR value.
 */
extern unsigned int DEV_at91_PIT_get_mode (void);

/**
 * PIT status
 * @return 0 not reached PIV : 1 reached PIV since last read.
 */
extern unsigned int DEV_at91_PIT_get_status (void);

/**
 * Read PIT CPIV and PICNT without resetting the counters
 * @return PIT_PIIR value.
 */
extern unsigned int DEV_at91_PIT_get_PIIR (unsigned int *picnt);

/**
 * Returns the value of the PIT Value Register, clearing it as a side effect.
 * @return PIT_PIVR value.
 */
extern unsigned int DEV_at91_PIT_get_PIVR (unsigned int *picnt);

extern unsigned int DEV_at91_PIT_ticks_2_usec (unsigned int pit_ticks);

#endif // DEV_AT91_PIT_H
