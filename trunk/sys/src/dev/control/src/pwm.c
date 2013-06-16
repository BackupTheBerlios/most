/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include "dev/pwm.h"

/* 36 values */
short DEV_pwm_sin[] = {
		63,62,61,58,54,49,44,38,
		32,26,20,15,10,6,3,2,
		1,2,3,6,10,15,20,26,
		32,38,44,49,54,58,61,62,
		0
};

extern void
DEV_pwm_init (DEV_pwm_t * pwm, struct DEV_pwm_interface *interface, ACE_u8_t channel)
{
    pwm->interface = interface;
    pwm->channel = channel;
}

extern void
DEV_pwm_set_period (DEV_pwm_t * pwm, ACE_u16_t period, ACE_u16_t duty)
{
    pwm->period = period;
    pwm->duty = duty;
    pwm->interface->set_period (pwm->channel, period, duty);
}

extern void
DEV_pwm_set_duty (DEV_pwm_t * pwm, ACE_u16_t duty)
{
    pwm->duty = duty;
    pwm->interface->set_duty (pwm->channel, pwm->period, duty);
}

extern void
DEV_pwm_enable (DEV_pwm_t * pwm)
{
    pwm->interface->enable (pwm->channel);
}

extern void
DEV_pwm_disable (DEV_pwm_t * pwm)
{
    pwm->interface->disable (pwm->channel);
}
