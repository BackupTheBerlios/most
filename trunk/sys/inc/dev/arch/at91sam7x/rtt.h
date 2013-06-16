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

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// Implementation of RTT Real Time Timer controller.
///
/// !Contents
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------


#ifndef DEV_AT91_RTT_H
#define DEV_AT91_RTT_H

#include "dev/arch/at91sam7x/AT91SAM7X256.h"

#define DEV_AT91_RTT_PRESCALER_1SEC   0x8000
/**
 * Changes the prescaler value of the given RTT and restarts it. This function
 * disables RTT interrupt sources.
 * @param rtt : Pointer to a AT91S_RTTC instance.
 * @param prescaler : Prescaler value for the RTT.
 */
extern void DEV_at91_RTT_set_prescaler (AT91S_RTTC * rtt, unsigned short prescaler);

/**
 * Returns the current value of the RTT timer value.
 * @param rtt : Pointer to a AT91S_RTTC instance.
 */
extern unsigned int DEV_at91_RTT_get_time (AT91S_RTTC * rtt);

/**
 * Enables the specified RTT interrupt sources.
 * @param rtt : Pointer to a AT91S_RTTC instance.
 * @param sources : Bitmask of interrupts to enable.
 */
extern void DEV_at91_RTT_enable_IT (AT91S_RTTC * rtt, unsigned int sources);

/**
 * Returns the status register value of the given RTT.
 * @param rtt : Pointer to an AT91S_RTTC instance.
 */
extern unsigned int DEV_at91_RTT_get_status (AT91S_RTTC * rtt);

/**
 * Configures the RTT to generate an alarm at the given time.
 * @param pRtt : Pointer to an AT91S_RTTC instance.
 * @param time : Alarm time.
 */
extern void DEV_at91_RTT_set_alarm (AT91S_RTTC * pRtt, unsigned int time);

#endif
