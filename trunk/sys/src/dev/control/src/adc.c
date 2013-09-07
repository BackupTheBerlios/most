/*
 * adc.c
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#include <ace/stdio.h>
#include <dev/adc.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

extern void
DEV_adc_init (DEV_adc_t *adc, void *rep, ACE_u32_t (*adc_get) (void *rep, enum DEV_adc_channel), ACE_u32_t ch_mask)
{
    adc->adc_get = adc_get;
    adc->channel_mask = ch_mask;
    adc->represent = rep;
    
}

extern ACE_u32_t
DEV_adc_get (DEV_adc_t * adc, enum DEV_adc_channel channel)
{
    if (channel < DEV_ADC_CHN_MAX){
        if ( 1 << channel & adc->channel_mask){
            return adc->adc_get (adc->represent, channel);
        }
    }
    return 0;
}

static void
info (MFS_descriptor_t * desc)
{
    DEV_adc_t *adc = (DEV_adc_t *) desc->represent;
    for (int i = 0; i < DEV_ADC_CHN_MAX; ++i){
        if (1 << i & adc->channel_mask){
            ACE_printf ("\n\tADC ch %i: %lu \n", i, DEV_adc_get(adc, i));
        }
    }
}

static struct MFS_descriptor_op adc_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
};

extern void
DEV_adc_install (DEV_adc_t * adc, char *name)
{
    adc->desc = MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/dev/control"), name,
                                    MFS_SYS, &adc_descriptor_op, (MFS_represent_t *) adc);
}

extern void
DEV_adc_remove (DEV_adc_t * adc)
{
    MFS_remove_desc (MFS_resolve(MFS_get_root(), "sys/dev/control"), adc->desc);
}
