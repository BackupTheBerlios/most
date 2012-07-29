/*
 * pwm.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef _SAM_PWM_H_
#define _SAM_PWM_H_

#include <dev/pwm.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup pwm pwm.h
 *
 * Puls with modulation.
 *
 * @{
 */

/**
 *
 */
extern DEV_pwm_t speaker;

/**
 *
 */
extern void SAM_pwm_init (void);

/**
 *
 */
extern void SAM_pwm_interrupt (void);

/**
 *
 */
extern void SAM_beep (void);

/** @}
 */

/** @}
 */

#endif /* _SAM_PWM_H_ */
