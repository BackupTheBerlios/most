/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <dev/arch/at91/pwm.h>
#include <dev/arch/at91/AT91SAM7X256.h>

#include "arch/cpu.h"

#define PWM_CHANNEL_SPEAKER 0
#define PWM_CHANNEL_SPEAKER_OUT_LENGTH 500

DEV_pwm_t speaker;

static struct DEV_pwm_interface at91_pwm;

extern void
SAM_pwm_init (void)
{
    at91_pwm.disable = DEV_at91_PWM_disable_channel;
    at91_pwm.enable = DEV_at91_PWM_enable_channel;
    at91_pwm.set_duty = DEV_at91_PWM_set_duty_cycle;
    at91_pwm.set_period = DEV_at91_PWM_set_period;

    DEV_at91_PWM_configure_clocks (4000000, 0, SAM_MCK);
    DEV_at91_PWM_configure_channel (PWM_CHANNEL_SPEAKER, AT91C_PWMC_CPRE_MCKA, 0, AT91C_PWMC_CPOL);

    DEV_pwm_init (&speaker, &at91_pwm, PWM_CHANNEL_SPEAKER);
    DEV_pwm_set_period (&speaker, 100);
    DEV_pwm_set_duty (&speaker, 100);
    DEV_pwm_disable (&speaker);

    DEV_at91_PWM_enable_channel_IT (PWM_CHANNEL_SPEAKER);
}

extern void
SAM_pwm_interrupt (void)
{
    // Interrupt on channel #0
    if ((AT91C_BASE_PWMC->PWMC_ISR & AT91C_PWMC_CHID0) == AT91C_PWMC_CHID0)
    {
        static unsigned int period_count = 0;
        static unsigned int period_index = 0;
        period_index++;
        if (DEV_pwm_sin[period_index] < 0)
        {
            period_index = 0;
            period_count++;
        }
        if (period_count >= PWM_CHANNEL_SPEAKER_OUT_LENGTH)
        {
            period_count = 0;
            DEV_pwm_disable (&speaker);
        }
        else
        {
            DEV_pwm_set_duty (&speaker, (speaker.period * DEV_pwm_sin[period_index]) / 100);
        }
    }
}

extern void
SAM_beep (void)
{
    DEV_pwm_enable (&speaker);
}
