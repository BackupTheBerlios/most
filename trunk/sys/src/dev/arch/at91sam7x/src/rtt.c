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

#include "dev/arch/at91sam7x/rtt.h"
#include <ace/assert.h>

void
DEV_at91_RTT_set_prescaler (AT91S_RTTC * rtt, unsigned short prescaler)
{
    rtt->RTTC_RTMR = (prescaler | AT91C_RTTC_RTTRST);
}

unsigned int
DEV_at91_RTT_get_time (AT91S_RTTC * rtt)
{
    return rtt->RTTC_RTVR;
}

void
DEV_at91_RTT_enable_IT (AT91S_RTTC * rtt, unsigned int sources)
{
    ACE_ASSERT ((sources & 0x0004FFFF) == 0, "RTT_EnableIT: Wrong sources value.\n");
    rtt->RTTC_RTMR |= sources;
}

unsigned int
DEV_at91_RTT_get_status (AT91S_RTTC * rtt)
{
    return rtt->RTTC_RTSR;
}

void
DEV_at91_RTT_set_alarm (AT91S_RTTC * pRtt, unsigned int time)
{
    ACE_SANITY_CHECK (time > 0);

    pRtt->RTTC_RTAR = time - 1;
}
