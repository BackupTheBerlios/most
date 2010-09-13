#ifndef DEV_AT91_ADC_H
#define DEV_AT91_ADC_H

#include <ace/stddef.h>
#include "dev/adc.h"

extern void DEV_at91_adc_init(void);
extern ACE_u32_t DEV_at91_adc_get(enum DEV_adc_channel channel);

#endif

