/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_DIGIO_H
#define SAM_DIGIO_H

#include <dev/digout.h>
#include <dev/digin.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup digio digio.h
 *
 * Digital Input,Output.
 *
 * @{
 */

/** Run Led. */
extern DEV_digout_t SAM_run_led;

/** Green Led. */
extern DEV_digout_t SAM_green_led;

/** Red Led. */
extern DEV_digout_t SAM_red_led;

/** LCD background light. */
extern DEV_digout_t SAM_lcd_light;

/** LCD reset. */
extern DEV_digout_t SAM_lcd_reset;

/** Ethernet PHY power down. */
extern DEV_digout_t SAM_phy_power_down;

/** Pin B29. */
extern DEV_digout_t SAM_pin_B29;



/** Switch bootloader. */
extern DEV_digin_t SAM_switch_boot;

/** Button left. */
extern DEV_digin_t SAM_switch_1;

/** Button right. */
extern DEV_digin_t SAM_switch_2;

/** SD card write protect. */
extern DEV_digin_t SAM_card_write;

/** SD card protect. */
extern DEV_digin_t SAM_card_insert;

/** Joystick up. */
extern DEV_digin_t SAM_joystick_up;

/** Joystick down. */
extern DEV_digin_t SAM_joystick_down;

/** Joystick left. */
extern DEV_digin_t SAM_joystick_left;

/** Joystick right. */
extern DEV_digin_t SAM_joystick_right;

/** Joystick button. */
extern DEV_digin_t SAM_joystick_center;

/** Pin B27. */
extern DEV_digin_t SAM_pin_B27;




/** Control panel inputs. */
extern DEV_diginputs_t SAM_control_in;


/**
 * Initialize digital io.
 */
extern void SAM_digio_init (void);

/**
 * Install digital io in sysfs.
 */
extern void SAM_digio_install (void);

/** @}
 */

/** @}
 */

#endif
