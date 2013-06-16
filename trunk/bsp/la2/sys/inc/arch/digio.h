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


/** SD card write protect. */
extern DEV_digin_t LA2_card_write;

/** SD card protect. */
extern DEV_digin_t LA2_card_insert;


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
