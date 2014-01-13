/*
 * ultra_sonic.c
 *
 *  Created on: 04.01.2014
 *      Author: maik
 */


#include <uso/cpu.h>
#include <mfs/descriptor.h>
#include <dev/cpu.h>
#include <sen/usonic_ranger.h>

#define SONIC_SPEED_MPS    343  /* meter / sec */


extern void
SEN_usonic_ranger_pulse (SEN_usonic_ranger_t *usonic, long duration)
{
    usonic->duration = duration;
    USO_go_all(&usonic->sync);
}

extern void
SEN_usonic_ranger_err (SEN_usonic_ranger_t *usonic)
{
    usonic->duration = 0;
    usonic->err++;
    USO_go_all(&usonic->sync);
}

extern void
SEN_usonic_ranger_init(SEN_usonic_ranger_t *usonic,
                       DEV_digout_t *out,
                       long clk,
                       void (*cfg_out) (ACE_u32_t, int),
                       void (*cfg_in) (ACE_u32_t),
                       ACE_u32_t pin)
{
    USO_barrier_init(&usonic->sync);
    usonic->out = out;
    usonic->clk = clk;
    usonic->cfg_out = cfg_out;
    usonic->cfg_in = cfg_in;
    usonic->pin = pin;
    usonic->duration = 0;
    usonic->err = 0;
    usonic->fact_cm = (SONIC_SPEED_MPS * 100 / 2);
}

extern void
SEN_usonic_ranger_measure_start(SEN_usonic_ranger_t *usonic)
{
    usonic->cfg_out(usonic->pin, 0);
    DEV_cpudelay(DEV_USEC_2_LOOPS(2));
    DEV_digout_set(usonic->out);
    DEV_cpudelay(DEV_USEC_2_LOOPS(5));
    USO_cpu_status_t ps = USO_disable ();
    DEV_digout_clear(usonic->out);
    usonic->cfg_in(usonic->pin);
    USO_restore (ps);
}

static long
usonic_ranger_calc_cm(SEN_usonic_ranger_t *usonic)
{
    long cm;
    USO_cpu_status_t ps = USO_disable ();
    cm = ((usonic->duration * usonic->fact_cm) / usonic->clk);
    USO_restore (ps);
    return cm;
}

extern long
SEN_usonic_ranger_get_cm(SEN_usonic_ranger_t *usonic)
{
    return usonic_ranger_calc_cm(usonic);
}

extern long
SEN_usonic_ranger_measure_cm(SEN_usonic_ranger_t *usonic)
{
    SEN_usonic_ranger_measure_start(usonic);
    USO_block(&usonic->sync);
    return usonic_ranger_calc_cm(usonic);
}


static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    SEN_usonic_ranger_t *usonic = (SEN_usonic_ranger_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "Distance";
            entry->value.l = SEN_usonic_ranger_measure_cm(usonic);
            break;
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "Error";
            entry->value.l = usonic->err;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};

extern void
SEN_usonic_ranger_install (SEN_usonic_ranger_t *usonic, MFS_descriptor_t * dir, char *name)
{
    MFS_descriptor_create (dir, name, MFS_USR, &descriptor_op, (MFS_represent_t *) usonic);
}
