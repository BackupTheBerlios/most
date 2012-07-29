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

#include "dev/arch/at91/pmc.h"
#include "dev/arch/at91/AT91SAM7X256.h"
#include <ace/assert.h>
#include <dev/debug.h>

#define MASK_STATUS 0x3FFFFFFC


void
DEV_at91_PMC_disable_processor_clock (void)
{
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_PCK;
    while ((AT91C_BASE_PMC->PMC_SCSR & AT91C_PMC_PCK) != AT91C_PMC_PCK);
}

void
DEV_at91_PMC_enable_peripheral (unsigned int id)
{
    ACE_SANITY_CHECK (id < 32);

    if ((AT91C_BASE_PMC->PMC_PCSR & (1 << id)) == (1 << id))
    {
        DEBUGF (DEV_AT91_DEBUG, ("PMC_EnablePeripheral: clock of peripheral"
                                 " %u is already enabled\n", id));
    }
    else
    {
        AT91C_BASE_PMC->PMC_PCER = 1 << id;
    }
}

void
DEV_at91_PMC_disable_peripheral (unsigned int id)
{
    ACE_SANITY_CHECK (id < 32);

    if ((AT91C_BASE_PMC->PMC_PCSR & (1 << id)) != (1 << id))
    {
        DEBUGF (DEV_AT91_DEBUG, ("PMC_DisablePeripheral: clock of peripheral"
                                 " %u is not enabled\n", id));
    }
    else
    {

        AT91C_BASE_PMC->PMC_PCDR = 1 << id;
    }
}

void
DEV_at91_PMC_enable_all_peripherals (void)
{
    AT91C_BASE_PMC->PMC_PCER = MASK_STATUS;
    while ((AT91C_BASE_PMC->PMC_PCSR & MASK_STATUS) != MASK_STATUS);
    DEBUGF (DEV_AT91_DEBUG, ("Enable all periph clocks\n"));
}

void
DEV_at91_PMC_disable_all_peripherals (void)
{
    AT91C_BASE_PMC->PMC_PCDR = MASK_STATUS;
    while ((AT91C_BASE_PMC->PMC_PCSR & MASK_STATUS) != 0);
    DEBUGF (DEV_AT91_DEBUG, ("Disable all periph clocks\n"));
}

unsigned int
DEV_at91_PMC_is_all_periph_enabled (void)
{
    return (AT91C_BASE_PMC->PMC_PCSR == MASK_STATUS);
}

unsigned int
DEV_at91_PMC_is_periph_enabled (unsigned int id)
{
    return (AT91C_BASE_PMC->PMC_PCSR & (1 << id));
}

void
DEV_at91_PMC_cpu_in_idle_mode (void)
{
    DEV_at91_PMC_disable_processor_clock ();
}
