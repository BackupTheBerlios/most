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
    USO_unlock(&mot->lock);
    USO_log_puts (USO_LL_INFO, "Motor is stopped.\n");
    return ACE_OK;
}

extern void
ACT_dc_motor_init (ACT_dc_motor_t *mot, DEV_pwm_t *U_out,
                DEV_digout_t *dir_a, DEV_digout_t *dir_b,
                int U_max, int dt, int du)
{
    mot->U_out = U_out;
    mot->dir_a = dir_a;
    mot->dir_b = dir_b;
    mot->U_max = U_max;
    mot->U_soll = 0;
    mot->U_ist = 0;
    mot->dt = dt;
    mot->du = du;
    mot->thread = NULL;
    USO_mutex_init(&mot->lock);
    USO_barrier_init(&mot->U_soll_changed);
    mot->state = ACT_DC_MOT_INIT;
    mot->desc = NULL;
}

extern void
ACT_dc_motor_start(ACT_dc_motor_t *mot, char *name)
{
    if (mot->state == ACT_DC_MOT_INIT || mot->state == ACT_DC_MOT_DEAD){
        mot->thread = USO_thread_new (motor_run,
                        MOTOR_STACK_SIZE,
                        USO_SYSTEM,
                        USO_FIFO,
                        name);
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

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    ACT_dc_motor_t *mot = (ACT_dc_motor_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "U max";
            entry->value.l = mot->U_max;
            break;
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "U ist";
            entry->value.l = mot->U_ist;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "U soll";
            entry->value.l = mot->U_soll;
            break;
        case 3:
            entry->type = MFS_INFO_LONG;
            entry->name = "dt";
            entry->value.l = mot->dt;
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "du";
            entry->value.l = mot->du;
            break;
        case 5:
            entry->type = MFS_INFO_LONG;
            entry->name = "State";
            entry->value.l = mot->state;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static void
control (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    ACT_dc_motor_t *mot = (ACT_dc_motor_t *) desc->represent;
    if (entry->type == MFS_CTRL_HELP){
        ACE_sprintf(entry->value.s, "\t 0   start\n"
                                    "\t 1   stop\n"
                                    "\t 2   break\n"
                                    "\t 3 l go\n");
        return;
    }
    switch (number){
        case 0:
            ACT_dc_motor_start(mot, desc->name);
            break;
        case 1:
            ACT_dc_motor_stop(mot);
            break;
        case 2:
            ACT_dc_motor_break(mot);
            break;
        case 3:
            if (entry->type == MFS_CTRL_LONG){
                ACT_dc_motor_go(mot, entry->value.l);
            }
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
ACT_dc_motor_install (ACT_dc_motor_t *mot, MFS_descriptor_t * dir, char *name)
{
    mot->desc = MFS_descriptor_create (dir, name, MFS_USR, &descriptor_op, (MFS_represent_t *) mot);
}


