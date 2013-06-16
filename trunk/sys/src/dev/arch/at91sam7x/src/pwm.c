#include <ace/assert.h>

#include "dev/err.h"
#include "dev/arch/at91sam7x/pwm.h"
#include "dev/arch/at91sam7x/AT91SAM7X256.h"
#include "dev/debug.h"

/**
 * Finds a prescaler/divisor couple to generate the desired frequency from
 * MCK.
 * Returns the value to enter in PWMC_MR or 0 if the configuration cannot be
 * met.
 * @param frequency : Desired frequency in Hz.
 * @param mck : Master clock frequency in Hz.
 */
static unsigned short
find_clock_configuration (unsigned int frequency, unsigned int mck)
{
    unsigned int divisors[11] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
    unsigned char divisor = 0;
    unsigned int prescaler;

    ACE_ASSERT ((frequency < mck), "Frequency >= MCK");

    // Find prescaler and divisor values
    prescaler = (mck / divisors[divisor]) / frequency;
    while ((prescaler > 255) && (divisor < 11))
    {

        divisor++;
        prescaler = (mck / divisors[divisor]) / frequency;
    }

    // Return result
    if (divisor < 11)
    {

        DEBUGF (DEV_AT91_DEBUG, ("Found divisor=%u and prescaler=%u for freq=%uHz\n\r",
                                 divisors[divisor], prescaler, frequency));

        return prescaler | (divisor << 8);
    }
    else
    {

        return 0;
    }
}

extern void
DEV_at91_PWM_configure_channel (unsigned char channel,
                                unsigned int prescaler,
                                unsigned int alignment, unsigned int polarity)
{
    ACE_ASSERT (prescaler < AT91C_PWMC_CPRE_MCKB, "Prescaler >= AT91C_PWMC_CPRE_MCKB.");
    ACE_ASSERT ((alignment & ~AT91C_PWMC_CALG) == 0, "Alignment invalid.");
    ACE_ASSERT ((polarity & ~AT91C_PWMC_CPOL) == 0, "Polarity invalid.");

    // Disable channel (effective at the end of the current period)
    if ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) != 0)
    {
        AT91C_BASE_PWMC->PWMC_DIS = 1 << channel;
        while ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) != 0);
    }

    // Configure channel
    AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CMR = prescaler | alignment | polarity;
}

extern void
DEV_at91_PWM_configure_clocks (unsigned int clka, unsigned int clkb, unsigned int mck)
{
    unsigned int mode = 0;
    unsigned int result;

    // Clock A
    if (clka != 0)
    {

        result = find_clock_configuration (clka, mck);
        ACE_ASSERT (result != 0, "Could not generate the desired PWM frequency (A).");
        mode |= result;
    }

    // Clock B
    if (clkb != 0)
    {

        result = find_clock_configuration (clkb, mck);
        ACE_ASSERT (result != 0, "Could not generate the desired PWM frequency (B).");
        mode |= (result << 16);
    }

    // Configure clocks
    DEBUGF (DEV_AT91_DEBUG, ("Setting PWMC_MR = 0x%08X.\n", mode));
    AT91C_BASE_PWMC->PWMC_MR = mode;
}

extern void
DEV_at91_PWM_set_period (unsigned char channel, unsigned short period, unsigned short duty)
{
    // If channel is disabled, write to CPRD
    if ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) == 0)
    {

        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CPRDR = period;
    }
    // Otherwise use update register
    else
    {

        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CMR |= AT91C_PWMC_CPD;
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CUPDR = period;
    }
}

extern void
DEV_at91_PWM_set_duty_cycle (unsigned char channel, unsigned short period, unsigned short duty)
{
    ACE_ASSERT (duty <= AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CPRDR, "Duty invalid.");

    // If channel is disabled, write to CDTY
    if ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) == 0)
    {

        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CDTYR = duty;
    }
    // Otherwise use update register
    else
    {

        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CMR &= ~AT91C_PWMC_CPD;
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CUPDR = duty;
    }
}

extern void
DEV_at91_PWM_enable_channel (unsigned char channel)
{
    AT91C_BASE_PWMC->PWMC_ENA = 1 << channel;
}

extern void
DEV_at91_PWM_disable_channel (unsigned char channel)
{
    AT91C_BASE_PWMC->PWMC_DIS = 1 << channel;
}

extern void
DEV_at91_PWM_enable_channel_IT (unsigned char channel)
{
    AT91C_BASE_PWMC->PWMC_IER = 1 << channel;
}

extern void
DEV_at91_PWM_disable_channel_IT (unsigned char channel)
{
    AT91C_BASE_PWMC->PWMC_IDR = 1 << channel;
}
