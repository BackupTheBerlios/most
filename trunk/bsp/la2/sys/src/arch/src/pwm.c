/*
 * pwm.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <uso/sleep.h>
#include <dev/arch/at91sam7a2/csp.h>

#include <arch/pwm.h>

#define PWM_CHANNEL_MOTOR1  0
#define PWM_CHANNEL_MOTOR2  1
#define PWM_CHANNEL_CH2     2
#define PWM_CHANNEL_BUZZER  3

#define PWM_DELAY_MOTOR        (PWM_PERIOD_MOTOR - PWM_DUTY_MOTOR)
#define PWM_PULSE_MOTOR        (PWM_DUTY_MOTOR)

#define PWM_DELAY_CH2          (PWM_PERIOD_CH2 - PWM_DUTY_CH2)
#define PWM_PULSE_CH2          (PWM_DUTY_CH2)

/* f = 3 kHZ, core_clk = 30 MHZ 
 * cnt_clk = f * period = 30 kHz
 * cnt_clk = core_clk / (2 ^ prescalar)
 * 29 kHz = 30 MHZ / 2 ^ 10
 * f = cnt_clk / period = 2,9 kHZ
 */
#define PWM_PRESCALAR_BUZZER   10
#define PWM_PERIOD_BUZZER      10
#define PWM_DUTY_BUZZER        5
#define PWM_DELAY_BUZZER       (PWM_PERIOD_BUZZER - PWM_DUTY_BUZZER)
#define PWM_PULSE_BUZZER       (PWM_DUTY_BUZZER)

#define PWM_INT_MASK        0     /* | PEND0 | PEND1 | PEND2 | PEND3 */

DEV_pwm_t LA2_motor1;
DEV_pwm_t LA2_motor2;
DEV_pwm_t LA2_ch2;
static DEV_pwm_t buzzer;

static struct DEV_pwm_interface pwm4c;


static void LA2_pwm_interrupt (void);

static void
PWM_enable_channel (unsigned char channel)
{
    CSP_PWM4CEnable(PWM0, channel);
}

static  void
PWM_disable_channel (unsigned char channel)
{
    CSP_PWM4CDisable(PWM0, channel);
}

static void
PWM_set_period (unsigned char channel, unsigned short period, unsigned short duty)
{
    CSP_PWM4C_CHANNEL_SET_DLY(PWM0, channel, period - duty);
    CSP_PWM4C_CHANNEL_SET_PUL(PWM0, channel, duty);
}

static void beep_handler (void)
{
}

extern void
LA2_pwm_init (void)
{
    pwm4c.disable = PWM_disable_channel;
    pwm4c.enable = PWM_enable_channel;
    pwm4c.set_period = PWM_set_period;
    pwm4c.set_duty = PWM_set_period;

    CSP_PWM4CInit(PWM0);
    CSP_PWM4CConfigureChannel(PWM0, PWM_CHANNEL_MOTOR1, (PWM_PRESCALAR_MOTOR | PL), PWM_DELAY_MOTOR, PWM_PULSE_MOTOR);
    CSP_PWM4CConfigureChannel(PWM0, PWM_CHANNEL_MOTOR2, (PWM_PRESCALAR_MOTOR | PL), PWM_DELAY_MOTOR, PWM_PULSE_MOTOR);
    CSP_PWM4CConfigureChannel(PWM0, PWM_CHANNEL_CH2,    (PWM_PRESCALAR_CH2 | PL), PWM_DELAY_CH2,    PWM_PULSE_CH2);
    CSP_PWM4CConfigureChannel(PWM0, PWM_CHANNEL_BUZZER, (PWM_PRESCALAR_BUZZER | PL), PWM_DELAY_BUZZER, PWM_PULSE_BUZZER);
    CSP_PWM4CConfigInterrupt(PWM0,(HIGH_LEVEL_SENSITIVE|PRIOR_3), PWM_INT_MASK, (U32_T) LA2_pwm_interrupt);

    DEV_pwm_init (&LA2_motor1, &pwm4c, PWM_CHANNEL_MOTOR1);
    DEV_pwm_set_period (&LA2_motor1, PWM_PERIOD_MOTOR, PWM_DUTY_MOTOR);
    DEV_pwm_init (&LA2_motor2, &pwm4c, PWM_CHANNEL_MOTOR2);
    DEV_pwm_set_period (&LA2_motor2, PWM_PERIOD_MOTOR, PWM_DUTY_MOTOR);
    DEV_pwm_init (&LA2_ch2, &pwm4c, PWM_CHANNEL_CH2);
    DEV_pwm_set_period (&LA2_ch2, PWM_PERIOD_CH2, PWM_DUTY_CH2);
    DEV_pwm_init (&buzzer, &pwm4c, PWM_CHANNEL_BUZZER);
    DEV_pwm_set_period (&buzzer, PWM_PERIOD_BUZZER, PWM_DUTY_BUZZER);
}



static void
LA2_pwm_interrupt (void)
{
    if ((CSP_PWM4C_GET_IMR(PWM0) & PEND0) == PEND0)
    {
        //motor1_handler();
    }
    if ((CSP_PWM4C_GET_IMR(PWM0) & PEND1) == PEND1)
    {
        //motor2_handler();
    }
    if ((CSP_PWM4C_GET_IMR(PWM0) & PEND2) == PEND2)
    {
        //ch2_handler();
    }
    if ((CSP_PWM4C_GET_IMR(PWM0) & PEND3) == PEND3)
    {
        beep_handler();
    }
}

extern void
LA2_beep (void)
{
    DEV_pwm_enable (&buzzer);
    USO_sleep (USO_MSEC_2_TICKS (1000));
    DEV_pwm_disable (&buzzer);
}
