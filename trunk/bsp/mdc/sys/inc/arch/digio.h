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
extern DEV_digout_t MDC_red_led;

/** Control Led 2 */
extern DEV_digout_t MDC_green_led;


/** Button */
extern DEV_digin_t MDC_button;

/** Jumper 1 */
extern DEV_digin_t MDC_jumper_1;

/** Jumper 2 */
extern DEV_digin_t MDC_jumper_2;

/** Jumper 3 */
extern DEV_digin_t MDC_jumper_3;

/** Switch */
extern DEV_digin_t MDC_switch;

/** Control inputs */
extern DEV_diginputs_t MDC_control_in;

/**
 * Initialize digital io.
 */
extern void MDC_digio_init (void);

/** @}
 */

/** @}
 */

#endif
