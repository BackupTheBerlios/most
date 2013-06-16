/*
 * pwm.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef LA2_PWM_H_
#define LA2_PWM_H_

#include <ace/stddef.h>
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
extern DEV_pwm_t buzzer;

/**
 *
 */
extern void LA2_pwm_init (void);


/**
 *
 */
extern void LA2_beep (void);

/** @}
 */

/** @}
 */

#endif /* _SAM_PWM_H_ */
