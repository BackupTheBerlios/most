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

#include <dev/arch/at91sam7x/rst.h>

/// WRITE_RSTC: Write RSTC register
#define WRITE_RSTC(pRstc, regName, value) pRstc->regName = (value)

/// READ_RSTC: Read RSTC registers
#define READ_RSTC(pRstc, regName) (pRstc->regName)

//-----------------------------------------------------------------------------
//         Defines
//-----------------------------------------------------------------------------

/// Keywords to write to the reset registers
#define RSTC_KEY_PASSWORD       (0xA5UL << 24)

void
DEV_at91_RST_configure_mode (AT91PS_RSTC rstc, unsigned int rmr)
{
    rmr &= ~AT91C_RSTC_KEY;
    WRITE_RSTC (rstc, RSTC_RMR, rmr | RSTC_KEY_PASSWORD);
}

void
DEV_at91_RST_set_user_reset_enable (AT91PS_RSTC rstc, unsigned char enable)
{
    unsigned int rmr = READ_RSTC (rstc, RSTC_RMR) & (~AT91C_RSTC_KEY);
    if (enable)
    {

        rmr |= AT91C_RSTC_URSTEN;
    }
    else
    {

        rmr &= ~AT91C_RSTC_URSTEN;
    }
    WRITE_RSTC (rstc, RSTC_RMR, rmr | RSTC_KEY_PASSWORD);
}

void
DEV_at91_RST_set_user_reset_IT_enable (AT91PS_RSTC rstc, unsigned char enable)
{
    unsigned int rmr = READ_RSTC (rstc, RSTC_RMR) & (~AT91C_RSTC_KEY);
    if (enable)
    {

        rmr |= AT91C_RSTC_URSTIEN;
    }
    else
    {

        rmr &= ~AT91C_RSTC_URSTIEN;
    }
    WRITE_RSTC (rstc, RSTC_RMR, rmr | RSTC_KEY_PASSWORD);
}

void
DEV_at91_RST_set_ext_reset_length (AT91PS_RSTC rstc, unsigned char powl)
{
    unsigned int rmr = READ_RSTC (rstc, RSTC_RMR);
    rmr &= ~(AT91C_RSTC_KEY | AT91C_RSTC_ERSTL);
    rmr |= (powl << 8) & AT91C_RSTC_ERSTL;
    WRITE_RSTC (rstc, RSTC_RMR, rmr | RSTC_KEY_PASSWORD);
}


void
DEV_at91_RST_processor_reset (AT91PS_RSTC rstc)
{
    WRITE_RSTC (rstc, RSTC_RCR, AT91C_RSTC_PROCRST | RSTC_KEY_PASSWORD);
}

void
DEV_at91_RST_peripheral_reset (AT91PS_RSTC rstc)
{
    WRITE_RSTC (rstc, RSTC_RCR, AT91C_RSTC_PERRST | RSTC_KEY_PASSWORD);
}

void
DEV_at91_RST_ext_reset (AT91PS_RSTC rstc)
{
    WRITE_RSTC (rstc, RSTC_RCR, AT91C_RSTC_EXTRST | RSTC_KEY_PASSWORD);
}

unsigned char
DEV_at91_RST_get_NRST_level (AT91PS_RSTC rstc)
{
    if (READ_RSTC (rstc, RSTC_RSR) & AT91C_RSTC_NRSTL)
    {

        return 1;
    }
    return 0;
}

unsigned char
DEV_at91_RST_is_user_reset_detected (AT91PS_RSTC rstc)
{
    if (READ_RSTC (rstc, RSTC_RSR) & AT91C_RSTC_URSTS)
    {

        return 1;
    }
    return 0;
}

unsigned char
DEV_at91_RST_is_busy (AT91PS_RSTC rstc)
{
    if (READ_RSTC (rstc, RSTC_RSR) & AT91C_RSTC_SRCMP)
    {

        return 1;
    }
    return 0;
}

unsigned int
DEV_at91_RST_status (AT91PS_RSTC rstc)
{
    return READ_RSTC (rstc, RSTC_RSR);
}
