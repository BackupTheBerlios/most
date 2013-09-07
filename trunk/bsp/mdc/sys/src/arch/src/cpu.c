/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <arch/cpu.h>

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
     * Area 0 (Flash) 2 state mode: The 55ns FlashRAM can operate in two state access mode   (125ns)
     * Area 5 (Ethernet) 2 state mode:
     * Area 7 (Ram) 2 state mode:
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
    H8_WCR = H8_WCR_RESERVE | H8_WCR_STATE_3;   // 3 wait states inserted
    H8_WCER = H8_WCER_WCE0 | H8_WCER_WCE1 | H8_WCER_WCE2 | H8_WCER_WCE3 |
        H8_WCER_WCE4 | H8_WCER_WCE5 | H8_WCER_WCE6 | H8_WCER_WCE7;

    H8_ITU_TMDR = 0;

}
