/*
 * adc.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <dev/arch/at91sam7x/adc.h>
#include <dev/adc.h>

#include "arch/spi.h"

DEV_adc_t adc;

extern void
SAM_adc_init (void)
{
    DEV_at91_ADC_init ();
    DEV_adc_init (&adc, DEV_at91_ADC_get);
}
