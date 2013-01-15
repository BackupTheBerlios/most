/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_H8300_PINS_H
#define MDC_H8300_PINS_H

#include <ace/stddef.h>
#include <dev/arch/h8/bits.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup pins pins.h
 *
 * IO port pins.
 *
 * @{
 */

#define H8_P42_GREEN_LED BIT(2)
#define H8_P43_RED_LED   BIT(3)
#define H8_P44_SWITCH    BIT(4)
#define H8_P45_WDI       BIT(5)
#define H8_P46_SEL0      BIT(6)
#define H8_P47_SEL1      BIT(7)

#define H8_P60_EE_CS     BIT(0)
#define H8_P61_EE_DO     BIT(1)
#define H8_P62_EE_CLK    BIT(2)

#define H8_P70_JUMPER_1  BIT(0)
#define H8_P71_JUMPER_2  BIT(1)
#define H8_P72_JUMPER_3  BIT(2)
#define H8_P76_BUTTON    BIT(6)
#define H8_P77_EE_DI     BIT(7)

#define H8_P84_CS0       BIT(4)

#define H8_PA1_RESETETH  BIT(1)
#define H8_PA4_PWM1      BIT(4)
#define H8_PA5_CTS0      BIT(5)

#define H8_PB0_PWM3      BIT(0)
#define H8_PB2_PWM4      BIT(2)
#define H8_PB7_RUN_LED   BIT(7)

#define H8_PC0_RTS0      BIT(0)
#define H8_PC1_RTS1      BIT(1)
#define H8_PC2_CTS1      BIT(2)
#define H8_PC3_CS5       BIT(3)
#define H8_PC4_CS6       BIT(4)
#define H8_PC5_CS7       BIT(5)

/**
 * Initializes the cpu pins.
 */
extern void MDC_pins_init (void);

/**
 * Triggers the external watchdog which should happen at least once every 1.6 sec.
 */
extern void MDC_watchdog_trigger (void)
ACE_SECTION_ (".ramcode");


/** @}
 */

/** @}
 */

#endif
