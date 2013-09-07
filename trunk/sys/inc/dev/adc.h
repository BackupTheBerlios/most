/*
 * adc.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef DEV_ADC_H_
#define DEV_ADC_H_

#include <ace/stddef.h>
#include <mfs/vfs.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup adc adc.h
 *
 * @{
 */

enum DEV_adc_channel
{
    DEV_ADC_CHN_0 = 0,
    DEV_ADC_CHN_1,
    DEV_ADC_CHN_2,
    DEV_ADC_CHN_3,
    DEV_ADC_CHN_4,
    DEV_ADC_CHN_5,
    DEV_ADC_CHN_6,
    DEV_ADC_CHN_7,
    DEV_ADC_CHN_MAX
};

struct DEV_adc
{
    ACE_u32_t channel_mask;
    MFS_descriptor_t *desc;
    void *represent;
    ACE_u32_t (*adc_get) (void *rep, enum DEV_adc_channel);
};

typedef struct DEV_adc DEV_adc_t;

extern void DEV_adc_init (DEV_adc_t *adc, void *rep, ACE_u32_t (*adc_get) (void *rep, enum DEV_adc_channel), ACE_u32_t ch_mask);

extern ACE_u32_t DEV_adc_get (DEV_adc_t * adc, enum DEV_adc_channel channel);

extern void DEV_adc_install (DEV_adc_t * adc, char *name);

extern void DEV_adc_remove (DEV_adc_t * adc);

/** @}
 */

/** @}
 */

#endif /* DEV_ADC_H_ */
