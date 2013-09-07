#ifndef DEV_AT91_ADC_H
#define DEV_AT91_ADC_H

#include <ace/stddef.h>
#include "dev/adc.h"

extern void DEV_at91_ADC_init (void);
extern ACE_u32_t DEV_at91_ADC_get (void *rep, enum DEV_adc_channel channel);

#endif
