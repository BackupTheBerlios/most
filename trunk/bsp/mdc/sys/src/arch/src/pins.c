/*
 * pins.c
 *
 *  Created on: 14.08.2012
 *      Author: maik
 */

#include <dev/arch/h83003/H83003_REG.h>
#include <dev/arch/h83003/H83003.h>
#include <arch/pins.h>
#include <dev/arch/h83003/bitops.h>

extern void
MDC_pins_init (void)
{
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


