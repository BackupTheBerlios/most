/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_DIGIO_H
#define MDC_DIGIO_H

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
extern DEV_digout_t MDC_run_led;

/** Ethernet Chip Reset */
extern DEV_digout_t MDC_eth_reset;

/** Control Led 1 */
extern DEV_digout_t MDC_ctrl_led_1;

/** Control Led 2 */
extern DEV_digout_t MDC_ctrl_led_2;

/** Control Led 3 */
extern DEV_digout_t MDC_ctrl_led_3;

/** Control Led 4 */
extern DEV_digout_t MDC_ctrl_led_4;

/** Control Led 5 */
extern DEV_digout_t MDC_ctrl_led_5;


/** Button */
extern DEV_digin_t MDC_button;

/** Jumper 1 */
extern DEV_digin_t MDC_jumper_1;

/** Jumper 2 */
extern DEV_digin_t MDC_jumper_2;

/** Jumper 3 */
extern DEV_digin_t MDC_jumper_3;

/** Jumper 4 */
extern DEV_digin_t MDC_jumper_4;


/**
 * Initialize digital io.
 */
extern void MDC_digio_init (void);

/** @}
 */

/** @}
 */

#endif
