/*
 * ultra_sonic.h
 *
 *  Created on: 04.01.2014
 *      Author: maik
 */

#ifndef SEN_ULTRA_SONIC_RANGER_H_
#define SEN_ULTRA_SONIC_RANGER_H_

#include <uso/barrier.h>
#include <dev/digout.h>

struct SEN_usonic_ranger
{
    USO_barrier_t sync;
    long volatile duration;
    unsigned long volatile err;
    long clk;
    long fact_cm;
    DEV_digout_t *out;
    void (*cfg_out) (ACE_u32_t, int);
    void (*cfg_in) (ACE_u32_t);
    ACE_u32_t pin;
};

typedef struct SEN_usonic_ranger SEN_usonic_ranger_t;

extern void
SEN_usonic_ranger_pulse (SEN_usonic_ranger_t *usonic, long duration);

extern void
SEN_usonic_ranger_err (SEN_usonic_ranger_t *usonic);

extern void
SEN_usonic_ranger_init(SEN_usonic_ranger_t *usonic,
                       DEV_digout_t *out,
                       long clk,
                       void (*cfg_out) (ACE_u32_t, int),
                       void (*cfg_in) (ACE_u32_t),
                       ACE_u32_t pin);

extern void
SEN_usonic_ranger_measure_start(SEN_usonic_ranger_t *usonic);

extern long
SEN_usonic_ranger_get_cm(SEN_usonic_ranger_t *usonic);

extern long
SEN_usonic_ranger_measure_cm(SEN_usonic_ranger_t *usonic);

extern void
SEN_usonic_ranger_install (SEN_usonic_ranger_t *usonic, MFS_descriptor_t * dir, char *name);


#endif /* ULTRA_SONIC_RANGER_H_ */
