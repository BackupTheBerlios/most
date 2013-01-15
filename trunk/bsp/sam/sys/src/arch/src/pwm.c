/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <uso/sleep.h>
#include <uso/barrier.h>
#include <dev/arch/at91/pwm.h>
#include <dev/arch/at91/AT91SAM7X256.h>

#include <arch/cpu.h>
#include <arch/adc.h>
#include <arch/digio.h>

#define PWM_CHANNEL_SPEAKER 0

DEV_pwm_t speaker;

static void (*interrupt_handler)(void) = NULL;
static struct DEV_pwm_interface at91_pwm;

static void beep_handler (void)
{
    static unsigned int period_index = 0;
    if (DEV_pwm_sin[period_index] <= 0)
    {
        period_index = 0;
    }
    DEV_pwm_set_duty (&speaker, DEV_pwm_sin[period_index]);
    period_index++;
}

extern void
SAM_pwm_init (void)
{
    at91_pwm.disable = DEV_at91_PWM_disable_channel;
    at91_pwm.enable = DEV_at91_PWM_enable_channel;
    at91_pwm.set_duty = DEV_at91_PWM_set_duty_cycle;
    at91_pwm.set_period = DEV_at91_PWM_set_period;

    DEV_at91_PWM_configure_clocks (1000000, 0, SAM_MCK);
    DEV_at91_PWM_configure_channel (PWM_CHANNEL_SPEAKER, AT91C_PWMC_CPRE_MCKA, 0, AT91C_PWMC_CPOL);

    DEV_pwm_init (&speaker, &at91_pwm, PWM_CHANNEL_SPEAKER);
    DEV_pwm_set_period (&speaker, 63);
    DEV_pwm_set_duty (&speaker, 63);
    DEV_pwm_disable (&speaker);
}



extern void
SAM_pwm_interrupt (void)
{
    // Interrupt on channel #0
    if ((AT91C_BASE_PWMC->PWMC_ISR & AT91C_PWMC_CHID0) == AT91C_PWMC_CHID0)
    {
    	if (interrupt_handler != NULL) interrupt_handler();
    }
}

extern void
SAM_beep (void)
{
	interrupt_handler = beep_handler;
    DEV_at91_PWM_enable_channel_IT (PWM_CHANNEL_SPEAKER);
    DEV_pwm_enable (&speaker);
	USO_sleep (USO_MSEC_2_TICKS (1000));
    DEV_pwm_disable (&speaker);
	DEV_at91_PWM_disable_channel_IT (PWM_CHANNEL_SPEAKER);
}

extern void
SAM_tone ()
{
    DEV_at91_PWM_disable_channel_IT (PWM_CHANNEL_SPEAKER);
    DEV_pwm_enable (&speaker);
    for (int i = 0; i < 10000; ++i ){
        DEV_digout_set (&SAM_red_led);
    	ACE_u32_t mic = DEV_adc_get (&adc, DEV_ADC_CHN_7);
        DEV_digout_clear (&SAM_red_led);
        int clean = mic -128;
    	if ( (clean <= -8) || (clean >= 8) ) {clean *= 4;}
    	else if ( (clean <= -4) || (clean >= 4) ) {clean *= 2;}
    	else if ( (clean <= -2) || (clean >= 2) ) {;}
    	else {clean = 0;}
    	DEV_pwm_set_duty (&speaker, ((clean + 128) / 4) & 63 );
    	USO_sleep (USO_USEC_2_TICKS (100));
    }
   	DEV_pwm_disable (&speaker);
}
