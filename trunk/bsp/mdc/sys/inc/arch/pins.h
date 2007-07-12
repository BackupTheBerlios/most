/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_H8300_PINS_H
#define MDC_H8300_PINS_H

#include <dev/arch/h8300/io_def.h>

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

#define H8_P40_SDA       BIT(0) ///< to SMOD type module ICC data signal
#define H8_P41_SCL       BIT(1) ///< to SMOD type module ICC clock signal
#define H8_P42_JTAG_TMS  BIT(2) ///< to EPM7064
#define H8_P43_JTAG_TCK  BIT(3) ///< to EPM7064
#define H8_P45_WDI       BIT(5) ///< to MAX691A trigger Watchdog signal
#define H8_P46_SEL0      BIT(6) ///< to 74HC4052 Multiplexer for SS0
#define H8_P47_SEL1      BIT(7) ///< to 74HC4052 Multiplexer for SS1
#define H8_P60_CS        BIT(0) ///< to serial EEPROM 93LC46 CS
#define H8_P61_DATA      BIT(1) ///< to serial EEPROM 93LC46 DI
#define H8_P62_CLK       BIT(2) ///< to serial EEPROM 93LC46 CLK
#define H8_P70_DR1       BIT(0) ///< from CN350 PIN 2
#define H8_P71_DR2       BIT(1) ///< from CN350 PIN 4
#define H8_P72_DR3       BIT(2) ///< from CN350 PIN 6
#define H8_P73_CNDRS     BIT(3) ///< from CN350 PIN 8
#define H8_P76_JAPO      BIT(6) ///< from Jackpotswitch
#define H8_P77_DATA      BIT(7) ///< from serial EEPROM 93LC46 DO
#define H8_P84_CS0       BIT(4) ///< to Chipselect signal for FLASH
#define H8_PA0_CN_TST    BIT(0) ///< to T360 to check CN350 connected
#define H8_PA1_RESETETH  BIT(1) ///< to LAN91C94 hard reset of eth chip
#define H8_PA4_PWM1      BIT(4)  
#define H8_PA5_CTS0      BIT(5) ///< to CN370(SS0) control signal for SCI0
#define H8_PA6_BS_OUT    BIT(6) ///< to CN500
#define H8_PB0_PWM3      BIT(0) ///< multiplexed with EEPROM DAT
#define H8_PB2_PWM4      BIT(2) ///< to CN100
#define H8_PB7_CPU_LED   BIT(7) ///< to green CPU LED
#define H8_PC1_RTS1      BIT(1) ///< from CN360(SS1) control signal for SCI1
#define H8_PC2_CTS1      BIT(2) ///< to CN360(SS1) control signal for SCI1
#define H8_PC3_CS5       BIT(3) ///< to Chipselect signal for ethernet chip Area 5
#define H8_PC4_CS6       BIT(4) ///< to Chipselect signal for expansion Area 6
#define H8_PC5_CS7       BIT(5) ///< through MAX691 chipselect signal for RAM Area 7
#define H8_PC0_RTS0      BIT(0) ///< from CN370(SS0) control signal for SCI0

/** @}
 */

/** @}
 */

#endif
