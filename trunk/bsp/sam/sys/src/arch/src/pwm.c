/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <dev/arch/at91/pwm.h>

DEV_pwm_t pwm;

extern void SAM_pwm_init(void)
{
	DEV_at91_pwm_init();
	DEV_pwm_init(&pwm, DEV_at91_pwm_set);
}
