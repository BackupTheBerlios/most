/*
 * servo.h
 *
 *  Created on: 17.12.2013
 *      Author: maik
 */

#ifndef ACT_RC_SERVO_H_
#define ACT_RC_SERVO_H_

#include <dev/pwm.h>

struct ACT_rc_servo{
    DEV_pwm_t *signal;
    int angle;
    int periode;
    int min;
    int max;
    int mid;
    int half;
    int pos;
};

typedef struct ACT_rc_servo ACT_rc_servo_t;

extern void
ACT_rc_servo_disable (ACT_rc_servo_t *servo);

extern void
ACT_rc_servo_enable (ACT_rc_servo_t *servo);

extern void
ACT_rc_servo_set_pos (ACT_rc_servo_t *servo, int angle);

extern void
ACT_rc_servo_init (ACT_rc_servo_t *servo, DEV_pwm_t *signal, int angle, int periode, int min, int max);

extern void
ACT_rc_servo_install (ACT_rc_servo_t *servo, MFS_descriptor_t * dir, char *name);

#endif /* SERVO_H_ */
