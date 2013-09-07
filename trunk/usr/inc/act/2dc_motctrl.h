/*
 * motctrl.h
 *
 *  Created on: 31.07.2013
 *      Author: maik
 */

#ifndef ACT_2DC_MOTCTRL_H_
#define ACT_2DC_MOTCTRL_H_

#include <mfs/vfs.h>
#include <act/dc_motor.h>

struct ACT_2dc_motctrl_io {
    DEV_pwm_t *m1;
    DEV_pwm_t *m2;
    DEV_digout_t *m1_dir_a;
    DEV_digout_t *m1_dir_b;
    DEV_digout_t *m2_dir_a;
    DEV_digout_t *m2_dir_b;
};

struct ACT_2dc_motctrl {
    ACT_dc_motor_t mot1;
    ACT_dc_motor_t mot2;
    int max_pos;
    int max_neg;
    int cur_speed;
    struct ACT_2dc_motctrl_io *io;
};

typedef struct ACT_2dc_motctrl ACT_2dc_motctrl_t;




extern void
ACT_2dc_mc_start(ACT_2dc_motctrl_t *ctr);

extern void
ACT_2dc_mc_stop(ACT_2dc_motctrl_t *ctr);

extern void
ACT_2dc_mc_go(ACT_2dc_motctrl_t *ctrl, int speed);

extern void
ACT_2dc_mc_turn(ACT_2dc_motctrl_t *ctrl, int speed, int turn);

extern void
ACT_2dc_mc_init (ACT_2dc_motctrl_t *ctrl, struct ACT_2dc_motctrl_io *io, int max, int dt, int du);


#endif /* MOTCTRL_H_ */
