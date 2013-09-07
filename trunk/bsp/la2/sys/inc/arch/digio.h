/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_DIGIO_H
#define LA2_DIGIO_H

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
extern DEV_digout_t LA2_run_led;

/** Green Led. */
extern DEV_digout_t LA2_green_led;

/** Red Led. */
extern DEV_digout_t LA2_red_led;

/** Ethernet PHY power down. */
extern DEV_digout_t LA2_lan_reset;

/** DC Motor 1 Direction A. */
extern DEV_digout_t LA2_m1_a;

/** DC Motor 1 Direction B. */
extern DEV_digout_t LA2_m1_b;

/** DC Motor 2 Direction A. */
extern DEV_digout_t LA2_m2_a;

/** DC Motor 2 Direction A. */
extern DEV_digout_t LA2_m2_b;

/** LED Bar 1 Clock. */
extern DEV_digout_t LA2_led_bar1_clk;

/** LED Bar 1 Data. */
extern DEV_digout_t LA2_led_bar1_data;

/** LED Bar 2 Clock. */
extern DEV_digout_t LA2_led_bar2_clk;

/** LED Bar 2 Data. */
extern DEV_digout_t LA2_led_bar2_data;

/** SD card write protect. */
extern DEV_digin_t LA2_card_write;

/** SD card protect. */
extern DEV_digin_t LA2_card_insert;

/** Switch. */
extern DEV_digin_t LA2_switch;

/** Pushbutton. */
extern DEV_digin_t LA2_pushbutton;

/** Line Sensor 1 Digital. */
extern DEV_digin_t LA2_line_1;

/** Line Sensor 2 Digital. */
extern DEV_digin_t LA2_line_2;


/** Control panel inputs. */
extern DEV_diginputs_t LA2_control_in;


/**
 * Initialize digital io.
 */
extern void LA2_digio_init (void);

/**
 * Install digital io in sysfs.
 */
extern void LA2_digio_install (void);

/** @}
 */

/** @}
 */

#endif
