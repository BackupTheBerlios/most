/*
 * adc.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef DEV_ADC_H_
#define DEV_ADC_H_

#include <ace/stddef.h>

enum DEV_adc_channel{
	DEV_ADC_CHN_0 = 0,
	DEV_ADC_CHN_1,
	DEV_ADC_CHN_2,
	DEV_ADC_CHN_3,
	DEV_ADC_CHN_4,
	DEV_ADC_CHN_5,
	DEV_ADC_CHN_6,
	DEV_ADC_CHN_7
};

struct PRI_adc{
	ACE_u32_t (*adc_get)(enum DEV_adc_channel);
};

typedef struct PRI_adc DEV_adc_t;

extern void DEV_adc_init(DEV_adc_t *adc, ACE_u32_t (*adc_get)(enum DEV_adc_channel));

extern ACE_u32_t DEV_adc_get(DEV_adc_t *adc, enum DEV_adc_channel channel);


#endif /* DEV_ADC_H_ */
