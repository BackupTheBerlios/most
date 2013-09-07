/*
 * adc.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef LA2_ADC_H_
#define LA2_ADC_H_

#include <dev/adc.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup adc adc.h
 *
 * ADC.
 *
 * @{
 */


/**
 *
 */
#define LA2_ADC0_RESOLUTION           1024

/**
 *
 */
extern DEV_adc_t adc0;

/**
 *
 */
extern void LA2_adc_install (void);

/** @} */

/** @} */

#endif /* LA2_ADC_H_ */
