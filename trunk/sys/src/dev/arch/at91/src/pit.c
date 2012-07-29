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

#include <ace/stddef.h>
#include "dev/arch/at91/pit.h"
#include "dev/arch/at91/AT91SAM7X256.h"

static unsigned int periodes_per_sec;
static unsigned int pit_freq;
static unsigned int pit_ticks_per_period;

extern void
DEV_at91_PIT_init (unsigned int period, unsigned int pit_frequency)
{
    periodes_per_sec = period;
    pit_freq = pit_frequency;
    pit_ticks_per_period = period ? (period * pit_frequency + 8) >> 4 : 0;      // +8 to avoid %10, /10
    AT91C_BASE_PITC->PITC_PIMR = pit_ticks_per_period;
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN;
}

extern void
DEV_at91_PIT_enable_IT (void)
{
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITIEN;
}

extern void
DEV_at91_PIT_disable_IT (void)
{
    AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITIEN;
}

extern unsigned int
DEV_at91_PIT_get_mode (void)
{
    return (AT91C_BASE_PITC->PITC_PIMR);
}

extern unsigned int
DEV_at91_PIT_get_status (void)
{
    return ((AT91C_BASE_PITC->PITC_PISR & AT91C_PITC_PITS) == AT91C_PITC_PITS);
}

extern unsigned int
DEV_at91_PIT_get_PIIR (unsigned int *picnt)
{
    unsigned int piir;
    piir = AT91C_BASE_PITC->PITC_PIIR;
    if (picnt != NULL)
    {
        *picnt = (piir & AT91C_PITC_PICNT) >> 20;
    }
    return piir & AT91C_PITC_CPIV;
}

extern unsigned int
DEV_at91_PIT_ticks_2_usec (unsigned int pit_ticks)
{
    unsigned int pit_ticks_per_sec = periodes_per_sec * pit_ticks_per_period;
    return (pit_ticks * DEV_AT91_PIT_SEC_IN_USEC) / pit_ticks_per_sec;
}

extern unsigned int
DEV_at91_PIT_get_PIVR (unsigned int *picnt)
{
    unsigned int pivr;
    pivr = AT91C_BASE_PITC->PITC_PIVR;
    if (picnt != NULL)
    {
        *picnt = (pivr & AT91C_PITC_PICNT) >> 20;
    }
    return pivr & AT91C_PITC_CPIV;
}
