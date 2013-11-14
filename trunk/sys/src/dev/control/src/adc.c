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
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    DEV_adc_t *adc = (DEV_adc_t *) desc->represent;
    if (number < DEV_ADC_CHN_MAX) {
        entry->type = MFS_INFO_SIZE;
        if (1 << number & adc->channel_mask){
            entry->name = "value";
            entry->value.z = DEV_adc_get(adc, number);
        } else {
            entry->name = "disabled";
            entry->value.z = 0;
        }
        
    } else {
        entry->type = MFS_INFO_NOT_AVAIL;
    }
}

static struct MFS_descriptor_op adc_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};

extern void
DEV_adc_install (DEV_adc_t * adc, char *name)
{
    MFS_descriptor_t * dir = MFS_resolve("/sys/dev/control");
    adc->desc = MFS_descriptor_create (dir, name,
                                    MFS_SYS, &adc_descriptor_op, (MFS_represent_t *) adc);
    MFS_close_desc(dir);
}

extern void
DEV_adc_remove (DEV_adc_t * adc)
{
    MFS_descriptor_t * dir = MFS_resolve("/sys/dev/control");
    MFS_remove_desc (dir, adc->desc);
    MFS_close_desc(dir);
}
