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

/** Run Led */
extern DEV_digout_t SAM_run_led;

/** Red Led */
extern DEV_digout_t SAM_red_led;

/** LCD background light */
extern DEV_digout_t SAM_lcd_light;

/** LCD background reset */
extern DEV_digout_t SAM_lcd_reset;


/** Button left */
extern DEV_digin_t SAM_switch_1;

/** Button right */
extern DEV_digin_t SAM_switch_2;

/** Joystick up */
extern DEV_digin_t SAM_joystick_up;

/** Joystick down */
extern DEV_digin_t SAM_joystick_down;

/** Joystick left */
extern DEV_digin_t SAM_joystick_left;

/** Joystick right */
extern DEV_digin_t SAM_joystick_right;

/** Joystick button */
extern DEV_digin_t SAM_joystick_center;

/** Control panel inputs */
extern DEV_diginputs_t SAM_control_in;

/**
 * Initialize digital io.
 */
extern void SAM_digio_init (void);

/** @}
 */

/** @}
 */

#endif
