/*
 * pwm.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef DEV_PWM_H_
#define DEV_PWM_H_

#include <ace/stddef.h>

extern short DEV_pwm_sin[];

struct PRI_pwm{
	 void (*pwm_set)(ACE_u32_t);
};

typedef struct PRI_pwm DEV_pwm_t;

extern void DEV_pwm_init(DEV_pwm_t *pwm, void (*pwm_set)(ACE_u32_t));

extern void DEV_pwm_set(DEV_pwm_t *pwm, ACE_u32_t val);

#endif /* DEV_PWM_H_ */
