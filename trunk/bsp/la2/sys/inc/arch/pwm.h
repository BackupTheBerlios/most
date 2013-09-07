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

/* f = 2 kHZ, core_clk = 30 MHZ, period = 100
 * cnt_clk = f * period = 200 kHz
 * cnt_clk = core_clk / (2 ^ prescalar)
 * 234 kHz = 30 MHZ / 2 ^ 7
 * f = cnt_clk / period = 2343 HZ
 */
#define PWM_PRESCALAR_MOTOR    7
#define PWM_PERIOD_MOTOR       100
#define PWM_DUTY_MOTOR         0 /* initial duty cycle, motor is not running */

#define PWM_PRESCALAR_CH2      10
#define PWM_PERIOD_CH2         10
#define PWM_DUTY_CH2           5

/**
 *
 */
extern DEV_pwm_t LA2_motor1;

/**
 *
 */
extern DEV_pwm_t LA2_motor2;

/**
 *
 */
extern DEV_pwm_t LA2_ch2;

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

#endif /* _LA2_PWM_H_ */
