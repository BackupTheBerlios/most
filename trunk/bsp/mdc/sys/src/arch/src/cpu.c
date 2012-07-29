/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "arch/cpu.h"

/**
 * The H8/3003 is set to CPU mode 3 by connecting the pins MD0 and MD1 to
 * Vcc and connecting MD2 to GND. This can be checked by reading the mode
 * control register but not changed (read only) Mode 3 is: 16Mbytes
 * address space, initial bus mode 8 bits, on-chip RAM enabled.
 */

void
MDC_cpu_init (void)
{
    /* Enable internal ram */
    H8_SYSCR = H8_SYSCR_RAME | H8_SYSCR_RESERVE;

    /* Disable all external interrupts */
    H8_IER = H8_IER_IRQDIS;

    H8_IPRA = H8_IPRA_IPRA7 | H8_IPRA_IPRA5 | H8_IPRA_IPRA3 | H8_IPRA_IPRA2 | H8_IPRA_IPRA0;
    H8_IPRB = H8_IPRB_IPRB3 | H8_IPRB_IPRB2;

    /*
     * When the bus width control register (ABWCR) designates areas 0 to 7 all as 8-bit-access-areas,
     * the H8/3003 operates in 8- bit bus mode and port 4 is a generic input/output port. When at 
     * least one of areas 0 to 7 is designated as 16-bit-access-area, the H8/3003 operates in 16-bit
     * bus mode and port 4 becomes the lower data bus. 
     */
    H8_ABWCR = H8_ABWCR_ABW0 | H8_ABWCR_ABW1 | H8_ABWCR_ABW2 | H8_ABWCR_ABW3 |
        H8_ABWCR_ABW4 | H8_ABWCR_ABW5 | H8_ABWCR_ABW6 | H8_ABWCR_ABW7;

    /*
     * The 55ns FlashRAM can operate in two state access mode   (125ns)
     */
    H8_ASTCR = H8_ASTCR_AST1 | H8_ASTCR_AST2 | H8_ASTCR_AST3 | H8_ASTCR_AST4 | H8_ASTCR_AST6;

    /*
     * The BRLE bit in the Bus release control register (BRCR) must be
     * set to 0 to have pins 1 + 2 of port6 as I/Os.
     */
    H8_BRCR = H8_BRCR_RESERVE;
    /*
     * All bits in the wait state control enable register (WCER) must be set to 1 and the WMS1 bit in
     * the wait control register must be 0 to have pin 0 of port 6 as I/O.
     */
    H8_WCR = H8_WCR_RESERVE | H8_WCR_STATE_2;   // 2 wait states inserted
    H8_WCER = H8_WCER_WCE0 | H8_WCER_WCE1 | H8_WCER_WCE2 | H8_WCER_WCE3 |
        H8_WCER_WCE4 | H8_WCER_WCE5 | H8_WCER_WCE6 | H8_WCER_WCE7;

    H8_ITU_TMDR = 0;

    H8_P4DR = 0;
    H8_P4DDR = (H8_P47_SEL1 | H8_P46_SEL0 | H8_P45_WDI | H8_P43_RED_LED | H8_P42_GREEN_LED);

    H8_P6DR = 0;
    H8_P6DDR = (H8_P62_EE_CLK | H8_P61_EE_DO | H8_P60_EE_CS);

    H8_P8DR = 0;
    H8_P8DDR = (H8_P84_CS0);

    H8_P9DR = 0;
    H8_P9DDR = (0x00);

    H8_PADR = 0;
    H8_PADDR = (H8_PA5_CTS0 | H8_PA1_RESETETH | H8_PA4_PWM1);

    H8_PBDR = 0;
    H8_PBDDR = (H8_PB7_RUN_LED | H8_PB0_PWM3 | H8_PB2_PWM4);

    H8_PCDR = 0;
    H8_PCDDR = (H8_PC5_CS7 | H8_PC4_CS6 | H8_PC3_CS5 | H8_PC2_CTS1 | H8_PC1_RTS1 | H8_PC0_RTS0);

    H8_BITCLEAR (H8_P46_SEL0, H8_P4DR);
    H8_BITCLEAR (H8_P47_SEL1, H8_P4DR);
}

extern void
MDC_watchdog_trigger (void)
{
    H8_BITNOT (H8_P45_WDI, H8_P4DR);
}
