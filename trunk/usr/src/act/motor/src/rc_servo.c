/*
 * servo.c
 *
 *  Created on: 31.07.2013
 *      Author: maik
 */

#include <ace/stddef.h>
#include <uso/log.h>
#include <mfs/descriptor.h>
#include <act/rc_servo.h>



extern void
ACT_rc_servo_disable (ACT_rc_servo_t *servo)
{
    DEV_pwm_set_period (servo->signal, servo->periode, 0);
    DEV_pwm_disable (servo->signal);
}

extern void
ACT_rc_servo_enable (ACT_rc_servo_t *servo)
{
    DEV_pwm_set_period (servo->signal, servo->periode, 0);
    DEV_pwm_enable (servo->signal);
}


extern void
ACT_rc_servo_set_pos (ACT_rc_servo_t *servo, int angle)
{
    if (angle > servo->angle) angle = servo->angle;
    if (angle < -servo->angle) angle = -servo->angle;
    servo->pos = servo->mid + ( (servo->half * angle) / servo->angle);
    DEV_pwm_set_period (servo->signal, servo->periode, servo->pos);
}

extern void
ACT_rc_servo_init (ACT_rc_servo_t *servo, DEV_pwm_t *signal, int angle, int periode, int min, int max)
{
    servo->signal = signal;
    servo->angle = angle;
    servo->periode = periode;
    servo->min = min;
    servo->max = max;
    servo->mid = (servo->min + servo->max) / 2;
    servo->half = servo->max - servo->mid;
    servo->pos = 0;
}

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    ACT_rc_servo_t *servo = (ACT_rc_servo_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "Angle";
            entry->value.l = servo->angle;
            break;
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "Period";
            entry->value.l = servo->periode;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "Min";
            entry->value.l = servo->min;
            break;
        case 3:
            entry->type = MFS_INFO_LONG;
            entry->name = "Max";
            entry->value.l = servo->max;
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "Pos";
            entry->value.l = servo->pos;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static void
control (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    ACT_rc_servo_t *servo = (ACT_rc_servo_t *) desc->represent;
    if (entry->type == MFS_CTRL_HELP){
        ACE_sprintf(entry->value.s, "\t 0   enable\n"
                                    "\t 1   disable\n"
                                    "\t 2 l pos\n"
                                    "\t 3 l angle\n"
                                    "\t 4 l periode\n"
                                    "\t 5 l min\n"
                                    "\t 6 l max\n");
        return;
    }
    switch (number){
        case 0:
            ACT_rc_servo_enable (servo);
            break;
        case 1:
            ACT_rc_servo_disable (servo);
            break;
        case 2:
            if (entry->type == MFS_CTRL_LONG){
                ACT_rc_servo_set_pos (servo, entry->value.l);
            }
            break;
        case 3:
            if (entry->type == MFS_CTRL_LONG){
                ACT_rc_servo_init (servo, servo->signal, entry->value.l, servo->periode, servo->min, servo->max);
            }
            break;
        case 4:
            if (entry->type == MFS_CTRL_LONG){
                ACT_rc_servo_init (servo, servo->signal, servo->angle, entry->value.l, servo->min, servo->max);
            }
            break;
        case 5:
            if (entry->type == MFS_CTRL_LONG){
                ACT_rc_servo_init (servo, servo->signal, servo->angle, servo->periode, entry->value.l, servo->max);
            }
            break;
        case 6:
            if (entry->type == MFS_CTRL_LONG){
                ACT_rc_servo_init (servo, servo->signal, servo->angle, servo->periode, servo->min, entry->value.l);
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
ACT_rc_servo_install (ACT_rc_servo_t *servo, MFS_descriptor_t * dir, char *name)
{
    MFS_descriptor_create (dir, name, MFS_USR, &descriptor_op, (MFS_represent_t *) servo);
}


