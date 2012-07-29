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

#include <ace/stddef.h>
#include <ace/assert.h>
#include <ace/string.h>


#include "dev/arch/at91/flashd.h"
#include "dev/arch/at91/efc.h"
#include "dev/debug.h"

/// Computes the lock range associated with the given address range.
static void
ComputeLockRange (unsigned int start,
                  unsigned int end, unsigned int *pActualStart, unsigned int *pActualEnd)
{
    AT91S_EFC *pStartEfc, *pEndEfc;
    unsigned short startPage, endPage;
    unsigned short numPagesInRegion;
    unsigned short actualStartPage, actualEndPage;

    // Convert start and end address in page numbers
    DEV_at91_EFC_translate_address (start, &pStartEfc, &startPage, 0);
    DEV_at91_EFC_translate_address (end, &pEndEfc, &endPage, 0);

    // Find out the first page of the first region to lock
    numPagesInRegion = AT91C_IFLASH_LOCK_REGION_SIZE / AT91C_IFLASH_PAGE_SIZE;
    actualStartPage = startPage - (startPage % numPagesInRegion);
    actualEndPage = endPage;
    if ((endPage % numPagesInRegion) != 0)
    {
        actualEndPage += numPagesInRegion - (endPage % numPagesInRegion);
    }

    // Store actual page numbers
    DEV_at91_EFC_compute_address (pStartEfc, actualStartPage, 0, pActualStart);
    DEV_at91_EFC_compute_address (pEndEfc, actualEndPage, 0, pActualEnd);
    DEBUGF (DEV_FLASH_DEBUG,
            ("Actual lock range is 0x%06X - 0x%06X\n\r", *pActualStart, *pActualEnd));
}

void
DEV_at91_FLASHD_initialize (unsigned int mck)
{
    DEV_at91_EFC_set_master_clock (mck);
    DEV_at91_EFC_set_erase_before_programming (AT91C_BASE_EFC, 1);
    DEV_at91_EFC_disable_IT (AT91C_BASE_EFC, AT91C_MC_FRDY | AT91C_MC_LOCKE | AT91C_MC_PROGE);
}

unsigned char
DEV_at91_FLASHD_erase (void)
{
    unsigned char error;

    error = DEV_at91_EFC_perform_command (AT91C_BASE_EFC, AT91C_MC_FCMD_ERASE_ALL, 0);
    return error;
}

static unsigned char pPageBuffer[AT91C_IFLASH_PAGE_SIZE];

unsigned char
DEV_at91_FLASHD_write (unsigned int address, const void *pBuffer, unsigned int size)
{
    AT91S_EFC *pEfc;
    unsigned short page;
    unsigned short offset;
    unsigned int writeSize;
    unsigned int pageAddress;
    unsigned short padding;
    unsigned char error;

    unsigned int sizeTmp;
    unsigned int *pAlignedDestination;
    unsigned int *pAlignedSource;

    ACE_SANITY_CHECK (address >= (unsigned int)AT91C_IFLASH);
    ACE_SANITY_CHECK (pBuffer);
    ACE_SANITY_CHECK ((address + size) <= (unsigned int)(AT91C_IFLASH + AT91C_IFLASH_SIZE));

    // Translate write address
    DEV_at91_EFC_translate_address (address, &pEfc, &page, &offset);

    // Write all pages
    while (size > 0)
    {

        // Copy data in temporary buffer to avoid alignment problems
        writeSize = ACE_MIN (AT91C_IFLASH_PAGE_SIZE - offset, size);
        DEV_at91_EFC_compute_address (pEfc, page, 0, &pageAddress);
        padding = AT91C_IFLASH_PAGE_SIZE - offset - writeSize;

        // Pre-buffer data (mask with 0xFF)
        memcpy (pPageBuffer, (void *)pageAddress, offset);

        // Buffer data
        memcpy (pPageBuffer + offset, pBuffer, writeSize);

        // Post-buffer data
        memcpy (pPageBuffer + offset + writeSize, (void *)(pageAddress + offset + writeSize),
                padding);

        // Write page
        // Writing 8-bit and 16-bit data is not allowed 
        // and may lead to unpredictable data corruption

        pAlignedDestination = (unsigned int *)pageAddress;
        pAlignedSource = (unsigned int *)pPageBuffer;
        sizeTmp = AT91C_IFLASH_PAGE_SIZE;
        while (sizeTmp >= 4)
        {
            *pAlignedDestination++ = *pAlignedSource++;
            sizeTmp -= 4;
        }

        // Send writing command
        error = DEV_at91_EFC_perform_command (pEfc, AT91C_MC_FCMD_START_PROG, page);
        if (error)
        {
            return error;
        }
        // Progression
        address += AT91C_IFLASH_PAGE_SIZE;
        pBuffer = (void *)((unsigned int)pBuffer + writeSize);
        size -= writeSize;
        page++;
        offset = 0;
    }

    return 0;
}

unsigned char
DEV_at91_FLASHD_lock (unsigned int start,
                      unsigned int end, unsigned int *pActualStart, unsigned int *pActualEnd)
{
    AT91S_EFC *pStartEfc, *pEndEfc, *pEfc;
    unsigned int actualStart, actualEnd;
    unsigned short startPage, endPage;
    unsigned char error;
    unsigned short numPagesInRegion = AT91C_IFLASH_LOCK_REGION_SIZE / AT91C_IFLASH_PAGE_SIZE;

    // Compute actual lock range and store it
    ComputeLockRange (start, end, &actualStart, &actualEnd);
    if (pActualStart)
    {

        *pActualStart = actualStart;
    }
    if (pActualEnd)
    {

        *pActualEnd = actualEnd;
    }

    // Compute page numbers
    DEV_at91_EFC_translate_address (actualStart, &pStartEfc, &startPage, 0);
    DEV_at91_EFC_translate_address (actualEnd, &pEndEfc, &endPage, 0);

    // Lock all pages
    // If there is an EFC crossover, lock all pages from first EFC
    pEfc = pEndEfc;

    // Lock remaining pages
    while (startPage < endPage)
    {

        error = DEV_at91_EFC_perform_command (pEfc, AT91C_MC_FCMD_LOCK, startPage);
        if (error)
        {

            return error;
        }
        startPage += numPagesInRegion;
    }

    return 0;
}

unsigned char
DEV_at91_FLASHD_unlock (unsigned int start,
                        unsigned int end, unsigned int *pActualStart, unsigned int *pActualEnd)
{
    AT91S_EFC *pStartEfc, *pEndEfc, *pEfc;
    unsigned int actualStart, actualEnd;
    unsigned short startPage, endPage;
    unsigned char error;
    unsigned short numPagesInRegion = AT91C_IFLASH_LOCK_REGION_SIZE / AT91C_IFLASH_PAGE_SIZE;

    // Compute actual unlock range and store it
    ComputeLockRange (start, end, &actualStart, &actualEnd);
    if (pActualStart)
    {

        *pActualStart = actualStart;
    }
    if (pActualEnd)
    {

        *pActualEnd = actualEnd;
    }

    // Compute page numbers
    DEV_at91_EFC_translate_address (actualStart, &pStartEfc, &startPage, 0);
    DEV_at91_EFC_translate_address (actualEnd, &pEndEfc, &endPage, 0);

    // Unlock all pages
    // If there is an EFC crossover, unlock all pages from first EFC
    pEfc = pEndEfc;

    // Unlock remaining pages
    while (startPage < endPage)
    {

        error = DEV_at91_EFC_perform_command (pEfc, AT91C_MC_FCMD_UNLOCK, startPage);
        if (error)
        {

            return error;
        }
        startPage += numPagesInRegion;
    }

    return 0;
}

unsigned char
DEV_at91_FLASHD_is_locked (unsigned int start, unsigned int end)
{
    AT91S_EFC *pStartEfc, *pEndEfc, *pEfc;
    unsigned short startPage, endPage;
    unsigned char startRegion, endRegion;
    unsigned int numPagesInRegion;
    unsigned int numLockedRegions = 0;
    unsigned int status;

    // Get EFC & page values
    DEV_at91_EFC_translate_address (start, &pStartEfc, &startPage, 0);
    DEV_at91_EFC_translate_address (end, &pEndEfc, &endPage, 0);

    // Compute region indexes
    numPagesInRegion = AT91C_IFLASH_LOCK_REGION_SIZE / AT91C_IFLASH_PAGE_SIZE;
    startRegion = startPage / numPagesInRegion;
    endRegion = endPage / numPagesInRegion;
    if ((endPage % numPagesInRegion) != 0)
    {

        endRegion++;
    }

    // EFC cross-over, handle starting page -> end page of EFC0
    pEfc = pEndEfc;

    // Remaining regions / no EFC cross-over
    status = DEV_at91_EFC_get_status (pEfc);
    while (startRegion < endRegion)
    {

        if ((status & (1 << startRegion << 16)) != 0)
        {
            numLockedRegions++;
        }
        startRegion++;
    }

    return numLockedRegions;
}

unsigned char
DEV_at91_FLASHD_is_GPNVM_set (unsigned char gpnvm)
{
    AT91S_EFC *pEfc = AT91C_BASE_EFC;
    unsigned int status;

    ACE_SANITY_CHECK (gpnvm < EFC_NUM_GPNVMS);

    // Check if GPNVM is set
    status = DEV_at91_EFC_get_status (pEfc);
    if ((status & (1 << gpnvm << 8)) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char
DEV_at91_FLASHD_set_GPNVM (unsigned char gpnvm)
{
    AT91S_EFC *pEfc = AT91C_BASE_EFC;

    ACE_SANITY_CHECK (gpnvm < EFC_NUM_GPNVMS);

    if (!DEV_at91_FLASHD_is_GPNVM_set (gpnvm))
    {
        return DEV_at91_EFC_perform_command (pEfc, AT91C_MC_FCMD_SET_GP_NVM, gpnvm);
    }
    else
    {
        return 0;
    }
}

unsigned char
DEV_at91_FLASHD_clear_GPNVM (unsigned char gpnvm)
{
    AT91S_EFC *pEfc = AT91C_BASE_EFC;

    ACE_SANITY_CHECK (gpnvm < EFC_NUM_GPNVMS);

    if (DEV_at91_FLASHD_is_GPNVM_set (gpnvm))
    {
        return DEV_at91_EFC_perform_command (pEfc, AT91C_MC_FCMD_CLR_GP_NVM, gpnvm);
    }
    else
    {
        return 0;
    }
}

unsigned char
DEV_at91_FLASHD_is_security_bit_set (void)
{

    AT91S_EFC *pEfc = AT91C_BASE_EFC;
    unsigned int status;

    status = DEV_at91_EFC_get_status (pEfc);
    return (((status & AT91C_MC_SECURITY) != 0) ? 1 : 0);

}

unsigned char
DEV_at91_FLASHD_set_security_bit (void)
{
    AT91S_EFC *pEfc = AT91C_BASE_EFC;

    if (DEV_at91_FLASHD_is_security_bit_set () == 0)
    {
        return DEV_at91_EFC_perform_command (pEfc, AT91C_MC_FCMD_SET_SECURITY, 0);
    }
    else
    {
        return 0;
    }
}
