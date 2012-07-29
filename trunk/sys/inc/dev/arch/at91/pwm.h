#ifndef DEV_AT91_PWM_H
#define DEV_AT91_PWM_H

#include <ace/stddef.h>
#include "dev/pwm.h"

/**
 * Configures PWM a channel with the given parameters.
 * The PWM controller must have been clocked in the PMC prior to calling this
 * function.
 * Beware: this function disables the channel. It waits until disable is effective.
 * @param channel : Channel number.
 * @param prescaler : Channel prescaler.
 * @param alignment : Channel alignment.
 * @param polarity : Channel polarity.
 */
void DEV_at91_PWM_configure_channel (unsigned char channel,
                                     unsigned int prescaler,
                                     unsigned int alignment, unsigned int polarity);

/**
 * Configures PWM clocks A & B to run at the given frequencies. This function
 * finds the best MCK divisor and prescaler values automatically.
 * @param clka : Desired clock A frequency (0 if not used).
 * @param clkb : Desired clock B frequency (0 if not used).
 * @param mck :  Master clock frequency.
 */
void DEV_at91_PWM_configure_clocks (unsigned int clka, unsigned int clkb, unsigned int mck);

/**
 * Sets the period value used by a PWM channel. This function writes directly
 * to the CPRD register if the channel is disabled; otherwise, it uses the
 * update register CUPD.
 * @param channel : Channel number.
 * @param period : Period value.
 */
void DEV_at91_PWM_set_period (unsigned char channel, unsigned short period);

/**
 * Sets the duty cycle used by a PWM channel. This function writes directly to
 * the CDTY register if the channel is disabled; otherwise it uses the
 * update register CUPD.
 * Note that the duty cycle must always be inferior or equal to the channel
 * period.
 * @param channel : Channel number.
 * @param duty : Duty cycle value.
 */
void DEV_at91_PWM_set_duty_cycle (unsigned char channel, unsigned short duty);

/**
 * Enables the given PWM channel. This does NOT enable the corresponding pin;
 * this must be done in the user code.
 * @param channel : Channel number.
 */
void DEV_at91_PWM_enable_channel (unsigned char channel);

/**
 * Disables the given PWM channel.
 * Beware, channel will be effectively disabled at the end of the current period.
 * Application can check channel is disabled using the following wait loop:
 * while ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) != 0);
 * @param channel : Channel number.
 */
void DEV_at91_PWM_disable_channel (unsigned char channel);


/**
 * Enables the period interrupt for the given PWM channel.
 * @param channel : Channel number.
 */
void DEV_at91_PWM_enable_channel_IT (unsigned char channel);

/**
 * Disables the period interrupt for the given PWM channel.
 * @param channel : Channel number.
 */
void DEV_at91_PWM_disable_channel_IT (unsigned char channel);

#endif
