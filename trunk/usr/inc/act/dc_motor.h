/*
 * dc_motor.h
 *
 *  Created on: 31.07.2013
 *      Author: maik
 */

#ifndef ACT_DC_MOTOR_H_
#define ACT_DC_MOTOR_H_

#include <ace/stddef.h>
#include <uso/thread.h>
#include <uso/monitor.h>
#include <dev/pwm.h>
#include <dev/digout.h>

enum ACT_dc_mot_state
{
    ACT_DC_MOT_INIT,      /**< Motor is initialized. */
    ACT_DC_MOT_RUN,       /**< Motor is running mode. */
    ACT_DC_MOT_BREAK,     /**< Motor is in break mode. */
    ACT_DC_MOT_STOP,      /**< Stop motor. */
    ACT_DC_MOT_DEAD       /**< Motor is stopped. */
};

struct ACT_dc_motor{
    DEV_pwm_t *U_out;
    DEV_digout_t *dir_a;
    DEV_digout_t *dir_b;
    int U_soll;
    int U_ist;
    int U_max;
    int du;
    int dt;
    char *name;
    USO_thread_t *thread;
    USO_mutex_t lock;
    USO_barrier_t U_soll_changed;
    enum ACT_dc_mot_state state;
};

typedef struct ACT_dc_motor ACT_dc_motor_t;


extern void
ACT_dc_motor_init(ACT_dc_motor_t *mot, DEV_pwm_t *U_out,
                DEV_digout_t *dir_a, DEV_digout_t *dir_b,
                int U_max, int dt, int du,
                char *name);

extern void
ACT_dc_motor_start(ACT_dc_motor_t *mot);

extern void
ACT_dc_motor_stop(ACT_dc_motor_t *mot);

extern void
ACT_dc_motor_break(ACT_dc_motor_t *mot);

extern void
ACT_dc_motor_go(ACT_dc_motor_t *mot, int U_soll);

#endif /* ACT_DC_MOTOR_H_ */
