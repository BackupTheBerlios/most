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
/*-----------------------------------------------------------------------------
 *  File Name           : lowlevel.c
 *  Object              : low level initialization file
 *  Creation            : FDy   20-Nov-2006
 *-----------------------------------------------------------------------------
 */

/* Include Standard files */
#include <dev/arch/at91/aic.h>
#include <dev/arch/at91/debug.h>

#include "arch/OLIMEX_SAM7_EX256.h"
#include "arch/ticks.h"
#include "arch/uart.h"

/*-----------------------------------------------------------------------------
 * Function Name       : pabt_handler
 * Object              : Prefetch Abort Handler
 *-----------------------------------------------------------------------------*/
void SAM_prefetch_abort(void)
{
	dbgu_print_ascii("-F- Prefetch abort at ");
	dbgu_print_hex8(AT91C_BASE_MC->MC_AASR);
	dbgu_print_ascii("\n");
	for(;;);
}

/*-----------------------------------------------------------------------------
 * Function Name       : dabt_handler
 * Object              : Data Abort Handler
 *-----------------------------------------------------------------------------*/
void SAM_data_abort(void)
{
	dbgu_print_ascii("-F- Data abort at ");
	dbgu_print_hex8(AT91C_BASE_MC->MC_AASR);
	dbgu_print_ascii("\n");
	for(;;);
}

/*-----------------------------------------------------------------------------
 * Function Name       : default_spurious_handler
 * Object              : default handler for spurious interrupt
 *-----------------------------------------------------------------------------*/
static void default_spurious_handler(void)
{
    dbgu_print_ascii("-F- Spurious Interrupt\n");
    while(1);
}

/*-----------------------------------------------------------------------------
 * Function Name       : default_fiq_handler
 * Object              : default handler for fast interrupt
 *-----------------------------------------------------------------------------*/
static void default_fiq_handler(void)
{
    dbgu_print_ascii("-F- Unexpected FIQ Interrupt\n");
    while(1);
}

/*-----------------------------------------------------------------------------
 * Function Name       : default_irq_handler
 * Object              : default handler for irq
 *-----------------------------------------------------------------------------*/
static void default_irq_handler(void)
{
    dbgu_print_ascii("-F- Unexpected IRQ Interrupt\n");
    while(1);
}

/*-----------------------------------------------------------------------------
 * Function Name       : default_irq_handler
 * Object              : default handler for irq
 *-----------------------------------------------------------------------------*/
static void sys_interrupt(void)
{
	if (AT91C_BASE_AIC->AIC_IPR & (1 << AT91C_ID_SYS) ){
		AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
		SAM_ticks_interrupt();
	}
	
	if (AT91C_BASE_AIC->AIC_IPR & (1 << AT91C_ID_US0) ) {
		AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_US0);
		SAM_uart_interrupt_0();
	}
}

void SAM_sys_interrupt_init(void)
{
	AIC_ConfigureIT(AT91C_ID_SYS, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x3 ), sys_interrupt);
	AIC_ConfigureIT(AT91C_ID_US0, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x4 ), sys_interrupt);
	AIC_EnableIT(AT91C_ID_SYS);
	AIC_EnableIT(AT91C_ID_US0);
}

/*-----------------------------------------------------------------------------
 * Function Name       : lowlevel_init
 * Object              : This function performs very low level HW initialization
 *                       this function can use a Stack, depending the compilation
 *                       optimization mode
 *-----------------------------------------------------------------------------*/
void SAM_cpu_init(void)
{
    unsigned char i = 0;

    ///////////////////////////////////////////////////////////////////////////
    // EFC Init
    ///////////////////////////////////////////////////////////////////////////
#ifdef AT91SAM7X512
    AT91C_BASE_MC->MC0_FMR = AT91C_MC_FWS_1FWS; // 1 Wait State to work at 48MHz
    AT91C_BASE_MC->MC1_FMR = AT91C_MC_FWS_1FWS; // 1 Wait State to work at 48MHz
#else
    AT91C_BASE_MC->MC_FMR = AT91C_MC_FWS_1FWS; // 1 Wait State to work at 48MHz
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Init PMC Step 1. Enable Main Oscillator
    // Main Oscillator startup time is board specific:
    // Main Oscillator Startup Time worst case (3MHz) corresponds to 15ms
    // (0x40 for AT91C_CKGR_OSCOUNT field)
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_PMC->PMC_MOR = (((AT91C_CKGR_OSCOUNT & (0x40 << 8)) | AT91C_CKGR_MOSCEN));
    // Wait Main Oscillator stabilization
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

    ///////////////////////////////////////////////////////////////////////////
    // Init PMC Step 2.
    // Set PLL to 96MHz (96,109MHz) and UDP Clock to 48MHz
    // PLL Startup time depends on PLL RC filter: worst case is choosen
    // UDP Clock (48,058MHz) is compliant with the Universal Serial Bus
    // Specification (+/- 0.25% for full speed)
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_PMC->PMC_PLLR = AT91C_CKGR_USBDIV_1           |
                               AT91C_CKGR_OUT_0              |
                               (16 << 8)                     |
                               (AT91C_CKGR_MUL & (72 << 16)) |
                               (AT91C_CKGR_DIV & 14);
    // Wait for PLL stabilization
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK));
    // Wait until the master clock is established for the case we already
    // turn on the PLL
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    ///////////////////////////////////////////////////////////////////////////
    // Init PMC Step 3.
    // Selection of Master Clock MCK equal to (Processor Clock PCK) PLL/2=48MHz
    // The PMC_MCKR register must not be programmed in a single write operation
    // (see. Product Errata Sheet)
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    // Wait until the master clock is established
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;
    // Wait until the master clock is established
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    ///////////////////////////////////////////////////////////////////////////
    // Reset AIC: assign default handler for each interrupt source
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_AIC->AIC_SVR[0] = (int) default_fiq_handler ;
    for (i = 1; i < 31; i++) {
        AT91C_BASE_AIC->AIC_SVR[i] = (int) default_irq_handler ;
    }
    AT91C_BASE_AIC->AIC_SPU = (unsigned int) default_spurious_handler;

    // Perform 8 IT acknoledge (write any value in EOICR)
    for (i = 0; i < 8 ; i++) {
        AT91C_BASE_AIC->AIC_EOICR = 0;
    }

    // Enable the Debug mode
    AT91C_BASE_AIC->AIC_DCR = AT91C_AIC_DCR_PROT;

    ///////////////////////////////////////////////////////////////////////////
    // Disable Watchdog
    ///////////////////////////////////////////////////////////////////////////
    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
}

void SAM_io_init (void)
{
	// for RunLED
	AT91C_BASE_PIOA->PIO_OER = (AT91C_PIO_PA3);		// set to output
	AT91C_BASE_PIOA->PIO_PER = (AT91C_PIO_PA3);		// set to PIO mode
	AT91C_BASE_PIOA->PIO_PPUDR = (AT91C_PIO_PA3);	// disable pull up

	// for RedLED
	AT91C_BASE_PIOA->PIO_OER = (AT91C_PIO_PA4);		// set to output
	AT91C_BASE_PIOA->PIO_PER = (AT91C_PIO_PA4);		// set to PIO mode
	AT91C_BASE_PIOA->PIO_PPUDR = (AT91C_PIO_PA4);	// disable pull up

	// for LCD Backlight
	AT91C_BASE_PIOB->PIO_OER = (AT91B_LCD_BL);		// set to output
	AT91C_BASE_PIOB->PIO_PER = (AT91B_LCD_BL);		// set to PIO mode
	AT91C_BASE_PIOB->PIO_PPUDR = (AT91B_LCD_BL);	// disable pull up

	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);	// enable periph clock for PIO controller
	AT91C_BASE_PIOA->PIO_ODR = (AT91A_JS_ALL);			// set PIO line to input
}
