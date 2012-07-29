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
/// Interface for configuration the Embedded Flash Controller (EFC) peripheral.
///
/// !Usage
///
/// -# Initialize system master clock of the EFC with EFC_SetMasterClock().
/// -# Enable/disable interrupt sources using EFC_EnableIt() and EFC_DisableIt().
/// -# Enables or disable the "Erase before programming" feature using 
///    EFC_SetEraseBeforeProgramming().
/// -# Translates the given address into which EFC, page and offset values for 
///    difference density %flash memory using EFC_TranslateAddress().
/// -# Computes the address of a %flash access given the EFC, page and offset 
///    for difference density %flash memory using EFC_ComputeAddress().
/// -# Start the executing command with EFC_StartCommand()
/// -# Retrieve the current status of the EFC using EFC_GetStatus().
///
//------------------------------------------------------------------------------
#ifndef DEV_AT91_EFC_H
#define DEV_AT91_EFC_H

#include "dev/arch/at91/AT91SAM7X256.h"
#include "dev/arch/at91/flashd.h"

#define EFC_NUM_GPNVMS          3

typedef struct _AT91S_EFC
{

    AT91_REG EFC_FMR;
    AT91_REG EFC_FCR;
    AT91_REG EFC_FSR;

} AT91S_EFC, *AT91PS_EFC;

#define AT91C_BASE_EFC       (AT91_CAST(AT91PS_EFC) 0xFFFFFF60)



/** Set master clock.
 *
 */
extern void DEV_at91_EFC_set_master_clock (unsigned int mck);

/** Enables the given interrupt sources on an EFC peripheral.
 *
 */
extern void DEV_at91_EFC_enable_IT (AT91S_EFC * pEfc, unsigned int sources);

/** Disables the given interrupt sources on an EFC peripheral.
 *
 */
extern void DEV_at91_EFC_disable_IT (AT91S_EFC * pEfc, unsigned int sources);

/** Enables or disable the "Erase before programming" feature of an EFC.
 *
 */
extern void DEV_at91_EFC_set_erase_before_programming (AT91S_EFC * pEfc, unsigned char enable);

/** Translates the given address into EFC, page and offset values. The resulting
 * values are stored in the provided variables if they are not null.
 */
extern void DEV_at91_EFC_translate_address (unsigned int address,
                                            AT91S_EFC ** ppEfc,
                                            unsigned short *pPage, unsigned short *pOffset);

/** Computes the address of a flash access given the EFC, page and offset.
 *
 */
extern void DEV_at91_EFC_compute_address (AT91S_EFC * pEfc,
                                          unsigned short page,
                                          unsigned short offset, unsigned int *pAddress);

/** Starts the executing the given command on an EFC. This function returns
 * as soon as the command is started. It does NOT set the FMCN field automatically.
 */
extern void
DEV_at91_EFC_start_command (AT91S_EFC * pEfc, unsigned char command, unsigned short argument)
ACE_SECTION_ (".ramcode");

/** Performs the given command and wait until its completion (or an error).
 *  Returns 0 if successful; otherwise returns an error code.
 */
     extern unsigned char DEV_at91_EFC_perform_command (AT91S_EFC * pEfc,
                                                        unsigned char command,
                                                        unsigned short argument)
    ACE_SECTION_ (".ramcode");

/** Returns the current status of an EFC. Keep in mind that this function clears
 *  the value of some status bits (LOCKE, PROGE).
 */
     extern unsigned int DEV_at91_EFC_get_status (AT91S_EFC * pEfc);

#endif
