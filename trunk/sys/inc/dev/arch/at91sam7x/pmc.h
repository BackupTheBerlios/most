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

#ifndef DEV_AT91_PMC_H
#define DEV_AT91_PMC_H

/**
 * Disables the processor clock
 */
extern void DEV_at91_PMC_disable_processor_clock (void);

/**
 * Enables the clock of a peripheral. The peripheral ID (AT91C_ID_xxx) is used
 * to identify which peripheral is targetted.
 * Note that the ID must NOT be shifted (i.e. 1 << AT91C_ID_xxx).
 * @param id : Peripheral ID (AT91C_ID_xxx).
 */
extern void DEV_at91_PMC_enable_peripheral (unsigned int id);

/**
 * Disables the clock of a peripheral. The peripheral ID (AT91C_ID_xxx) is used
 * to identify which peripheral is targetted.
 * Note that the ID must NOT be shifted (i.e. 1 << AT91C_ID_xxx).
 * @param id : Peripheral ID (AT91C_ID_xxx).
 */
extern void DEV_at91_PMC_disable_peripheral (unsigned int id);

/**
 * Put the CPU in Idle Mode for lower consumption
 */
extern void DEV_at91_PMC_cpu_in_idle_mode (void);

/**
 * Enable all the periph clock via PMC
 * (Becareful of the last 2 bits, it is not periph clock)
 */
extern void DEV_at91_PMC_enable_all_peripherals (void);

/**
 * Disable all the periph clock via PMC
 * (Becareful of the last 2 bits, it is not periph clock)
 */
extern void DEV_at91_PMC_disable_all_peripherals (void);

/**-----------------------------------------------------------------------------
 * Get Periph Status
 */
extern unsigned int DEV_at91_PMC_is_all_periph_enabled (void);

/**
 * Get Periph Status
 */
extern unsigned int DEV_at91_PMC_is_periph_enabled (unsigned int id);

#endif
