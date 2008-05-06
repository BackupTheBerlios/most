/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation
 *
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

#include "dev/arch/at91/debug.h"
#include "dev/arch/at91/AT91SAM7X256.h"

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_ascii
 * Object              : This function is used to send a string through the
 *                       DBGU channel (Very low level debugging)
 *----------------------------------------------------------------------------*/
void dbgu_print_ascii(const char *buffer)
{
    while (*buffer != '\0') {
        while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
        AT91C_BASE_DBGU->DBGU_THR = (*buffer++ & 0x1FF);
    }
}

/*----------------------------------------------------------------------------
 * Function Name       : dbgu_print_hex8
 * Object              : This function is used to print a 32-bit value in hexa
 *----------------------------------------------------------------------------*/
void dbgu_print_hex8(unsigned long value)
{
    char c = 0;
    char shift = sizeof(unsigned long) * 8;

    dbgu_print_ascii("0x");
    do {
        shift -= 4;
        while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
        c = ((value >> shift) & 0xF);
        if (c > 9)
	  AT91C_BASE_DBGU->DBGU_THR = (('A' + (c - 10)) & 0x1FF);
        else
	  AT91C_BASE_DBGU->DBGU_THR = (('0' + c) & 0x1FF);
    } while (shift != 0);
}

/*-----------------------------------------------------------------------------
 * Function Name       : configure_dbgu
 * Object              : Configure DBGU
 *-----------------------------------------------------------------------------*/
void configure_dbgu (void)
{
    /* Reset and disable receiver */
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX;

    /* Disable interrupts */
    AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;

    /* Configure PIOs for DBGU */
    AT91C_BASE_PIOA->PIO_ASR = AT91C_PA27_DRXD | AT91C_PA28_DTXD;
    AT91C_BASE_PIOA->PIO_BSR = 0;
    AT91C_BASE_PIOA->PIO_PDR = AT91C_PA27_DRXD | AT91C_PA28_DTXD;

    /* === Configure serial link === */
    /* Define the baud rate divisor register [BRGR = MCK / (115200 * 16)] */
    AT91C_BASE_DBGU->DBGU_BRGR = 26;
    /* Define the USART mode */
    AT91C_BASE_DBGU->DBGU_MR = AT91C_US_PAR_NONE | AT91C_US_CHMODE_NORMAL;

    /* Disable the RX and TX PDC transfer requests */
    AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTDIS;
    AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTDIS;

    /* Enable transmitter */
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
}
