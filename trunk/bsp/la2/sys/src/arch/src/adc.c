/*
 * adc.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <dev/arch/at91sam7a2/csp.h>
#include <dev/adc.h>

DEV_adc_t adc0;

extern void
LA2_adc_install (void)
{
    CSP_ADC8CInit(ADC0, (PRLVAL(9) | STARTUPTIME(0x78)) );
    CSP_ADC8CEnable(ADC0);
    DEV_adc_init (&adc0, ADC0, SAM7A2_adc_get,
                    1 << DEV_ADC_CHN_0 |
                    1 << DEV_ADC_CHN_1);
    DEV_adc_install (&adc0, "adc0");
}
