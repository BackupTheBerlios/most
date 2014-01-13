/*
 * encoder.c
 *
 *  Created on: 25.12.2013
 *      Author: maik
 */

#include <uso/cpu.h>
#include <mfs/descriptor.h>
#include <sen/encoder.h>



extern void
SEN_encoder_tick(SEN_encoder_t *enc, ACE_u16_t period)
{
    enc->period = period;
    enc->counter++;
    if ( (enc->pos != 0) && (enc->counter >= enc->pos) ){
        USO_go_all(&enc->sync);
    }
}

extern void
SEN_encoder_err(SEN_encoder_t *enc)
{
    enc->period = 0;
    enc->err++;
}

extern void
SEN_encoder_init(SEN_encoder_t *enc, long clk_hz, int resolution, int gear, int d_mm)
{
    USO_barrier_init(&enc->sync);
    enc->clk = clk_hz;
    enc->resolution = resolution;
    enc->gear = gear;
    enc->d_mm = d_mm;
    enc->counter = 0;
    enc->err = 0;
    enc->pos = 0;
    enc->period = 0;
    enc->rpm_fact = (enc->clk * 60) / (enc->resolution * enc->gear);
    enc->U_mm = ( enc->d_mm * 314 ) / 100;
}

extern long
SEN_encoder_get_n_rpm(SEN_encoder_t *enc)
{
    long n = 0;
    USO_cpu_status_t ps = USO_disable ();
    ACE_u16_t p = enc->period;
    USO_restore (ps);
    if (p){
        n = enc->rpm_fact / p;
    }
    return n;
}

extern long
SEN_encoder_get_v_mmps(SEN_encoder_t *enc)
{
    long n = SEN_encoder_get_n_rpm(enc);
    return (n * enc->U_mm) / 60;
}

extern ACE_u32_t
SEN_encoder_get_l_mm(SEN_encoder_t *enc)
{
    USO_cpu_status_t ps = USO_disable ();
    ACE_u32_t c = enc->counter;
    USO_restore (ps);
    return  (c * enc->U_mm) / (enc->resolution * enc->gear);
}

extern void
SEN_encoder_clear_counter(SEN_encoder_t *enc)
{
    USO_cpu_status_t ps = USO_disable ();
    enc->counter = 0;
    enc->err = 0;
    enc->period = 0;
    if (enc->pos){
        USO_go_all(&enc->sync);
    }
    USO_restore (ps);
}

extern void
SEN_encoder_wait_pos_mm(SEN_encoder_t *enc, ACE_u32_t pos)
{
    USO_cpu_status_t ps = USO_disable ();
    enc->pos = (pos * (enc->resolution * enc->gear)) / enc->U_mm;
    USO_block(&enc->sync);
    enc->pos = 0;
    USO_restore (ps);
}

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    SEN_encoder_t *enc = (SEN_encoder_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "Period";
            entry->value.l = enc->period;
            break;
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "Counter";
            entry->value.l = enc->counter;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "Error";
            entry->value.l = enc->err;
            break;
        case 3:
            entry->type = MFS_INFO_LONG;
            entry->name = "n_rpm";
            entry->value.l = SEN_encoder_get_n_rpm(enc);
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "v_mmps";
            entry->value.l = SEN_encoder_get_v_mmps(enc);
            break;
        case 5:
            entry->type = MFS_INFO_SIZE;
            entry->name = "l_mm";
            entry->value.z = SEN_encoder_get_l_mm(enc);
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static void
control (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    SEN_encoder_t *enc = (SEN_encoder_t *) desc->represent;
    if (entry->type == MFS_CTRL_HELP){
        ACE_sprintf(entry->value.s, "\t 0   clear_cntr\n"
                                    "\t 1 z wait pos\n");
        return;
    }
    switch (number){
        case 0:
            SEN_encoder_clear_counter (enc);
            break;
        case 1:
            SEN_encoder_wait_pos_mm (enc, entry->value.z);
            break;
        default:
            break;
    }
}

static struct MFS_descriptor_op descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = control,
    .delete = NULL
};

extern void
SEN_encoder_install (SEN_encoder_t *enc, MFS_descriptor_t * dir, char *name)
{
    MFS_descriptor_create (dir, name, MFS_USR, &descriptor_op, (MFS_represent_t *) enc);
}

