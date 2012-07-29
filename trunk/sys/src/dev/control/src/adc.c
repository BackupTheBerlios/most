/*
 * adc.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include "dev/adc.h"

extern void
DEV_adc_init (DEV_adc_t * adc, ACE_u32_t (*adc_get) (enum DEV_adc_channel))
{
    adc->adc_get = adc_get;
}

extern ACE_u32_t
DEV_adc_get (DEV_adc_t * adc, enum DEV_adc_channel channel)
{
    return adc->adc_get (channel);
}
