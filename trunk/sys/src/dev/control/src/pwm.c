/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include "dev/pwm.h"

/* 36 values */
short DEV_pwm_sin[] = {
    100, 99, 97, 94, 89, 82, 75, 67, 59,
    50, 41, 33, 25, 18, 11, 6, 3, 1,
    0, 1, 3, 6, 11, 18, 25, 33, 41,
    50, 59, 67, 75, 82, 89, 94, 97, 99,
    -1
};

extern void
DEV_pwm_init (DEV_pwm_t * pwm, struct DEV_pwm_interface *interface, ACE_u8_t channel)
{
    pwm->interface = interface;
    pwm->channel = channel;
}

extern void
DEV_pwm_set_period (DEV_pwm_t * pwm, ACE_u16_t period)
{
    pwm->period = period;
    pwm->interface->set_period (pwm->channel, period);
}

extern void
DEV_pwm_set_duty (DEV_pwm_t * pwm, ACE_u16_t duty)
{
    pwm->duty = duty;
    pwm->interface->set_duty (pwm->channel, duty);
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
