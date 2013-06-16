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
//         Headers
//------------------------------------------------------------------------------

#include <ace/assert.h>
#include <uso/cpu.h>

#include "dev/arch/at91sam7x/efc.h"
#include "dev/debug.h"

// Round a number to the nearest integral value (number must have been
// multiplied by 10, e.g. to round 10.3 enter 103).
#define ROUND(n)    ((((n) % 10) >= 5) ? (((n) / 10) + 1) : ((n) / 10))

// Returns the FMCN field value when manipulating lock bits, given MCK.
#define FMCN_BITS(mck)      (ROUND((mck) / 100000) << 16)

// Returns the FMCN field value when manipulating the rest of the flash.
#define FMCN_FLASH(mck)         ((((mck) / 2000000) * 3) << 16)

static unsigned int lMck = 0;

extern void
DEV_at91_EFC_set_master_clock (unsigned int mck)
{
    lMck = mck;
}

void
DEV_at91_EFC_enable_IT (AT91S_EFC * pEfc, unsigned int sources)
{
    ACE_SANITY_CHECK (pEfc);
    ACE_SANITY_CHECK ((sources & ~0x0000000D) == 0);

    pEfc->EFC_FMR |= sources;
}

void
DEV_at91_EFC_disable_IT (AT91S_EFC * pEfc, unsigned int sources)
{
    ACE_SANITY_CHECK (pEfc);
    ACE_SANITY_CHECK ((sources & ~(AT91C_MC_FRDY | AT91C_MC_LOCKE | AT91C_MC_PROGE)) == 0);

    pEfc->EFC_FMR &= ~sources;
}

void
DEV_at91_EFC_set_erase_before_programming (AT91S_EFC * pEfc, unsigned char enable)
{
    ACE_SANITY_CHECK (pEfc);

    if (enable)
    {

        pEfc->EFC_FMR &= ~AT91C_MC_NEBP;
    }
    else
    {

        pEfc->EFC_FMR |= AT91C_MC_NEBP;
    }
}

void
DEV_at91_EFC_translate_address (unsigned int address,
                                AT91S_EFC ** ppEfc, unsigned short *pPage, unsigned short *pOffset)
{
    AT91S_EFC *pEfc;
    unsigned short page;
    unsigned short offset;

    ACE_SANITY_CHECK (address >= (unsigned int)AT91C_IFLASH);
    ACE_SANITY_CHECK (address <= (unsigned int)(AT91C_IFLASH + AT91C_IFLASH_SIZE));

    pEfc = AT91C_BASE_EFC;
    page = ((char *)address - AT91C_IFLASH) / AT91C_IFLASH_PAGE_SIZE;
    offset = ((char *)address - AT91C_IFLASH) % AT91C_IFLASH_PAGE_SIZE;

    DEBUGF (DEV_FLASH_DEBUG, ("Translated 0x%08X to EFC=0x%08X, page=%d and offset=%d\n",
                              address, (unsigned int)pEfc, page, offset));

    // Store values
    if (ppEfc)
    {

        *ppEfc = pEfc;
    }
    if (pPage)
    {

        *pPage = page;
    }
    if (pOffset)
    {

        *pOffset = offset;
    }
}

void
DEV_at91_EFC_compute_address (AT91S_EFC * pEfc,
                              unsigned short page, unsigned short offset, unsigned int *pAddress)
{
    unsigned int address;

    ACE_SANITY_CHECK (pEfc);
    ACE_SANITY_CHECK (page <= AT91C_IFLASH_NB_OF_PAGES);
    ACE_SANITY_CHECK (offset < AT91C_IFLASH_PAGE_SIZE);

    // Compute address
    address = (unsigned int)(AT91C_IFLASH + page * AT91C_IFLASH_PAGE_SIZE + offset);

    // Store result
    if (pAddress)
    {

        *pAddress = address;
    }
}

void
DEV_at91_EFC_start_command (AT91S_EFC * pEfc, unsigned char command, unsigned short argument)
{
    ACE_SANITY_CHECK (pEfc);
    ACE_ASSERT (lMck != 0, "Master clock not set.\n");

    USO_cpu_status_t ps;
    ps = USO_disable ();

    // Check command & argument
    switch (command)
    {

    case AT91C_MC_FCMD_PROG_AND_LOCK:
        ACE_ASSERT (0, "Write and lock command cannot be carried out.\n");
        break;
    case AT91C_MC_FCMD_START_PROG:
    case AT91C_MC_FCMD_LOCK:
    case AT91C_MC_FCMD_UNLOCK:
        ACE_ASSERT (argument < AT91C_IFLASH_NB_OF_PAGES, "Maximum number of pages\n");
    	break;
    case AT91C_MC_FCMD_SET_GP_NVM:
    case AT91C_MC_FCMD_CLR_GP_NVM:
        ACE_ASSERT (argument < EFC_NUM_GPNVMS, "A maximum of GPNVMs are available on the chip.\n");
        break;
    case AT91C_MC_FCMD_ERASE_ALL:
    case AT91C_MC_FCMD_SET_SECURITY:
        ACE_ASSERT (argument == 0, "Argument is meaningless for the given command\n");
        break;
    default:
    	ACE_ASSERT (0, "Unknown command\n");
    	/* no break */
    }

    // Set FMCN
    switch (command)
    {

    case AT91C_MC_FCMD_LOCK:
    case AT91C_MC_FCMD_UNLOCK:
    case AT91C_MC_FCMD_SET_GP_NVM:
    case AT91C_MC_FCMD_CLR_GP_NVM:
    case AT91C_MC_FCMD_SET_SECURITY:
        pEfc->EFC_FMR = (pEfc->EFC_FMR & ~AT91C_MC_FMCN) | FMCN_BITS (lMck);
        break;

    case AT91C_MC_FCMD_START_PROG:
    case AT91C_MC_FCMD_ERASE_ALL:
        pEfc->EFC_FMR = (pEfc->EFC_FMR & ~AT91C_MC_FMCN) | FMCN_FLASH (lMck);
        break;
    default:
    	break;
    }

    // Start command
    ACE_ASSERT ((pEfc->EFC_FSR & AT91C_MC_FRDY) != 0, "Efc is not ready\n");
    pEfc->EFC_FCR = (0x5A << 24) | (argument << 8) | command;

    USO_restore (ps);
}

unsigned char
DEV_at91_EFC_perform_command (AT91S_EFC * pEfc, unsigned char command, unsigned short argument)
{
    unsigned int status;

    USO_cpu_status_t ps;
    ps = USO_disable ();

    // Set FMCN
    switch (command)
    {

    case AT91C_MC_FCMD_LOCK:
    case AT91C_MC_FCMD_UNLOCK:
    case AT91C_MC_FCMD_SET_GP_NVM:
    case AT91C_MC_FCMD_CLR_GP_NVM:
    case AT91C_MC_FCMD_SET_SECURITY:
        pEfc->EFC_FMR = (pEfc->EFC_FMR & ~AT91C_MC_FMCN) | FMCN_BITS (lMck);
        break;

    case AT91C_MC_FCMD_START_PROG:
    case AT91C_MC_FCMD_ERASE_ALL:
        pEfc->EFC_FMR = (pEfc->EFC_FMR & ~AT91C_MC_FMCN) | FMCN_FLASH (lMck);
        break;
    }

    pEfc->EFC_FCR = (0x5A << 24) | (argument << 8) | command;
    do
    {

        status = pEfc->EFC_FSR;
    }
    while ((status & AT91C_MC_FRDY) == 0);

    USO_restore (ps);

    return (status & (AT91C_MC_PROGE | AT91C_MC_LOCKE));
}

unsigned int
DEV_at91_EFC_get_status (AT91S_EFC * pEfc)
{
    return pEfc->EFC_FSR;
}
