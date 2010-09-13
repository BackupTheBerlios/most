/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include "dev/pwm.h"

short DEV_pwm_sin[] = {
512, 579, 645, 708, 768, 824, 874, 918, 955, 985, 1007, 1020, 1023, 1020, 1007, 985, 955, 918, 874, 824, 768, 708, 645, 579,
512, 445, 379, 316, 256, 200, 150, 106, 69, 39, 17, 4, 0, 4, 17, 39, 69, 106, 150, 200, 256, 316, 379, 445, -1};

extern void DEV_pwm_init(DEV_pwm_t *pwm, void (*pwm_set)(ACE_u32_t))
{
	pwm->pwm_set = pwm_set;
}

extern void DEV_pwm_set(DEV_pwm_t *pwm, ACE_u32_t val)
{
	pwm->pwm_set(val);
}
