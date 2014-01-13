/*
 * encoder.h
 *
 *  Created on: 25.12.2013
 *      Author: maik
 */

#ifndef SEN_ENCODER_H_
#define SEN_ENCODER_H_

#include <ace/stddef.h>
#include <uso/barrier.h>

struct SEN_encoder
{
    USO_barrier_t sync;
    ACE_u16_t volatile period;
    ACE_u32_t volatile counter;
    ACE_u32_t volatile err;
    ACE_u32_t pos;
    long clk;
    int resolution;
    int gear;
    long rpm_fact;
    int d_mm;
    int U_mm;
};

typedef struct SEN_encoder SEN_encoder_t;

/**
 * For sync this function must be called within an interrupt or with interrupts disabled!
 */
extern void
SEN_encoder_tick(SEN_encoder_t *enc, ACE_u16_t period);

/**
 * For sync this function must be called within an interrupt or with interrupts disabled!
 */
extern void
SEN_encoder_err(SEN_encoder_t *enc);

extern void
SEN_encoder_init(SEN_encoder_t *enc, long clk_hz, int resolution, int gear, int d_mm);

extern long
SEN_encoder_get_n_rpm(SEN_encoder_t *enc);

extern long
SEN_encoder_get_v_mmps(SEN_encoder_t *enc);

extern ACE_u32_t
SEN_encoder_get_l_mm(SEN_encoder_t *enc);

extern void
SEN_encoder_clear_counter(SEN_encoder_t *enc);

extern void
SEN_encoder_wait_pos_mm(SEN_encoder_t *enc, ACE_u32_t pos);

extern void
SEN_encoder_install (SEN_encoder_t *enc, MFS_descriptor_t * dir, char *name);

#endif /* ENCODER_H_ */
