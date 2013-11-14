/*
 * dc_motor.c
 *
 *  Created on: 31.07.2013
 *      Author: maik
 */

#include <ace/stddef.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/digout.h>
#include <act/dc_motor.h>

#define MOTOR_STACK_SIZE      (0x400/sizeof(USO_stack_t))


/* this function is maybe called in the context of an interrupt */
static void
motor_cleanup (void *arg)
{
    ACT_dc_motor_t *mot = (ACT_dc_motor_t *) arg;
    mot->U_soll = 0;
    mot->U_ist = 0;
    DEV_pwm_set_period (mot->U_out, mot->U_max, 0);
    DEV_digout_clear(mot->dir_a);
    DEV_digout_clear(mot->dir_b);
    DEV_pwm_disable (mot->U_out);
    mot->state = ACT_DC_MOT_DEAD;
}

static ACE_err_t
motor_run (void *arg)
{
    ACT_dc_motor_t *mot = (ACT_dc_motor_t *) arg;
    USO_lock(&mot->lock);
    mot->U_soll = 0;
    mot->U_ist = 0;
    DEV_pwm_set_period (mot->U_out, mot->U_max, 0);
    DEV_digout_clear(mot->dir_a);
    DEV_digout_clear(mot->dir_b);
    DEV_pwm_enable (mot->U_out);
    USO_cleanup_install (motor_cleanup);
    mot->state = ACT_DC_MOT_RUN;
    USO_log_puts (USO_LL_INFO, "Motor is running.\n");
    for (;!(mot->state == ACT_DC_MOT_STOP && mot->U_ist == 0);)
    {
        switch (mot->state){
            case ACT_DC_MOT_RUN:
                while (mot->U_ist == mot->U_soll && mot->state == ACT_DC_MOT_RUN){
                    USO_monitor (&mot->lock, &mot->U_soll_changed);
                }
                break;
            case ACT_DC_MOT_BREAK:
                while (mot->U_ist == mot->U_soll && mot->state == ACT_DC_MOT_BREAK){
                    USO_monitor (&mot->lock, &mot->U_soll_changed);
                }
                break;
            case ACT_DC_MOT_STOP:
                break;
            default:
                break;
        }
        if (mot->U_ist == 0){
            if (mot->U_soll > 0){
                DEV_digout_clear(mot->dir_a);
                DEV_digout_set(mot->dir_b);
            } else if (mot->U_soll <  0){
                DEV_digout_clear(mot->dir_b);
                DEV_digout_set(mot->dir_a);
            } else if (mot->state == ACT_DC_MOT_BREAK){
                DEV_pwm_disable (mot->U_out);
                DEV_digout_set(mot->dir_a);
                DEV_digout_set(mot->dir_b);
            }
        }
        int du = 0;
        if (mot->U_ist <= mot->U_soll){
            du = ACE_MIN(mot->du, mot->U_soll - mot->U_ist);
        } else {
            du = -ACE_MIN(mot->du, mot->U_ist - mot->U_soll);
        }
        if (mot->U_ist < 0 && mot->U_ist + du > 0){
            mot->U_ist = 0;
        } else if (mot->U_ist > 0 && mot->U_ist + du < 0){
            mot->U_ist = 0;
        } else {
            mot->U_ist += du;
        }
        if (mot->U_ist >= 0){
            DEV_pwm_set_period (mot->U_out, mot->U_max, mot->U_ist);
        } else {
            DEV_pwm_set_period (mot->U_out, mot->U_max, -mot->U_ist);
        }
        USO_unlock(&mot->lock);
        USO_sleep (USO_MSEC_2_TICKS (mot->dt));
        USO_lock(&mot->lock);
    }
    motor_cleanup (mot);
    USO_unlock(&mot->lock);
    USO_log_puts (USO_LL_INFO, "Motor is stopped.\n");
    return ACE_OK;
}

extern void
ACT_dc_motor_init (ACT_dc_motor_t *mot, DEV_pwm_t *U_out,
                DEV_digout_t *dir_a, DEV_digout_t *dir_b,
                int U_max, int dt, int du,
                char *name)
{
    mot->U_out = U_out;
    mot->dir_a = dir_a;
    mot->dir_b = dir_b;
    mot->U_max = U_max;
    mot->dt = dt;
    mot->du = du;
    mot->thread = NULL;
    USO_mutex_init(&mot->lock);
    USO_barrier_init(&mot->U_soll_changed);
    mot->state = ACT_DC_MOT_INIT;
    mot->name = name;
}

extern void
ACT_dc_motor_start(ACT_dc_motor_t *mot)
{
    if (mot->state == ACT_DC_MOT_INIT || mot->state == ACT_DC_MOT_DEAD){
        mot->thread = USO_thread_new (motor_run,
                        MOTOR_STACK_SIZE,
                        USO_SYSTEM,
                        USO_FIFO,
                        mot->name);
        if (mot->thread){
            USO_thread_arg_init (mot->thread, mot);
            USO_thread_flags_set (mot->thread, 1 << USO_FLAG_DETACH);
            USO_log_puts (USO_LL_INFO, "Start motor.\n");
            USO_start (mot->thread);
        }
    }
}

extern void
ACT_dc_motor_stop(ACT_dc_motor_t *mot)
{
    if (mot->state == ACT_DC_MOT_RUN || mot->state == ACT_DC_MOT_BREAK){
        USO_log_puts (USO_LL_INFO, "Stop motor.\n");
        USO_lock(&mot->lock);
        mot->U_soll = 0;
        mot->state = ACT_DC_MOT_STOP;
        USO_unlock(&mot->lock);
        USO_go_all(&mot->U_soll_changed);
    }
}

extern void
ACT_dc_motor_break(ACT_dc_motor_t *mot)
{
    if (mot->state == ACT_DC_MOT_RUN){
        USO_lock(&mot->lock);
        mot->U_soll = 0;
        mot->state = ACT_DC_MOT_BREAK;
        USO_unlock(&mot->lock);
        USO_go_all(&mot->U_soll_changed);
    }
}

extern void
ACT_dc_motor_go(ACT_dc_motor_t *mot, int U_soll)
{
    if (mot->state == ACT_DC_MOT_RUN || mot->state == ACT_DC_MOT_BREAK){
        USO_lock(&mot->lock);
        if (mot->state == ACT_DC_MOT_BREAK){
            DEV_pwm_enable (mot->U_out);
        }
        if (U_soll > mot->U_max){
            U_soll = mot->U_max;
        }
        if (U_soll < -mot->U_max){
            U_soll = -mot->U_max;
        }
        mot->U_soll = U_soll;
        mot->state = ACT_DC_MOT_RUN;
        USO_unlock(&mot->lock);
        USO_go_all(&mot->U_soll_changed);
    }
}

