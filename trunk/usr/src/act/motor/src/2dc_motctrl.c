/*
 * motctrl.c
 *
 *  Created on: 31.07.2013
 *      Author: maik
 */


#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <mfs/descriptor.h>
#include <act/2dc_motctrl.h>



extern void
ACT_2dc_mc_start(ACT_2dc_motctrl_t *ctrl)
{
    ctrl->cur_speed = 0;
    ACT_dc_motor_start(&ctrl->mot1, ctrl->mot1.desc != NULL ? ctrl->mot1.desc->name : "mot1");
    ACT_dc_motor_start(&ctrl->mot2, ctrl->mot2.desc != NULL ? ctrl->mot2.desc->name : "mot2");
}


extern void
ACT_2dc_mc_stop(ACT_2dc_motctrl_t *ctrl)
{
    ACT_dc_motor_stop(&ctrl->mot1);
    ACT_dc_motor_stop(&ctrl->mot2);
    ctrl->cur_speed = 0;
}



static int
check_speed(ACT_2dc_motctrl_t *ctrl, int speed)
{
    if (speed > ctrl->max_pos){
        speed = ctrl->max_pos;
    } else if (speed < ctrl->max_neg){
        speed = ctrl->max_neg;
    }
    return speed;
}



extern void
ACT_2dc_mc_go(ACT_2dc_motctrl_t *ctrl, int speed, int turn)
{
    int m1turn;
    int m2turn;
    ctrl->cur_speed = check_speed(ctrl, speed);
    m1turn = ctrl->cur_speed - turn;
    m2turn = ctrl->cur_speed + turn;
    if (m1turn > ctrl->max_pos){
        m2turn -= m1turn - ctrl->max_pos;
        m1turn = ctrl->max_pos;
    } else if (m1turn < ctrl->max_neg){
        m2turn -= m1turn - ctrl->max_neg;
        m1turn = ctrl->max_neg;
    } else if (m2turn > ctrl->max_pos){
        m1turn -= m2turn - ctrl->max_pos;
        m2turn = ctrl->max_pos;
    } else if (m2turn < ctrl->max_neg){
        m1turn -= m2turn - ctrl->max_neg;
        m2turn = ctrl->max_neg;
    }
    ACT_dc_motor_go(&ctrl->mot1, m1turn);
    ACT_dc_motor_go(&ctrl->mot2, m2turn);
}


extern void
ACT_2dc_mc_init (ACT_2dc_motctrl_t *ctrl, struct ACT_2dc_motctrl_io *io, int max, int dt, int du){
    ctrl->io = io;
    ctrl->max_pos = max;
    ctrl->max_neg = -max;
    ctrl->cur_speed = 0;
    ACT_dc_motor_init(&ctrl->mot1, ctrl->io->m1, ctrl->io->m1_dir_a, ctrl->io->m1_dir_b, max, dt, du);
    ACT_dc_motor_init(&ctrl->mot2, ctrl->io->m2, ctrl->io->m2_dir_a, ctrl->io->m2_dir_b, max, dt, du);
}

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    ACT_2dc_motctrl_t *ctrl = (ACT_2dc_motctrl_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "Pos max";
            entry->value.l = ctrl->max_pos;
            break;
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "Neg max";
            entry->value.l = ctrl->max_neg;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "Speed curr";
            entry->value.l = ctrl->cur_speed;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static void
control (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    ACT_2dc_motctrl_t *ctrl = (ACT_2dc_motctrl_t *) desc->represent;
    if (entry->type == MFS_CTRL_HELP){
        ACE_sprintf(entry->value.s, "\t 0   start\n"
                                    "\t 1   stop\n"
                                    "\t 2 l go\n"
                                    "\t 3 l turn\n");
        return;
    }
    switch (number){
        case 0:
            ACT_2dc_mc_start(ctrl);
            break;
        case 1:
            ACT_2dc_mc_stop(ctrl);
            break;
        case 2:
            if (entry->type == MFS_CTRL_LONG){
                ACT_2dc_mc_go(ctrl, entry->value.l, 0);
            }
            break;
        case 3:
            if (entry->type == MFS_CTRL_LONG){
                ACT_2dc_mc_go(ctrl, ctrl->cur_speed, entry->value.l);
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
ACT_2dc_mc_install (ACT_2dc_motctrl_t *ctrl, MFS_descriptor_t * dir, char *name)
{
    ACT_dc_motor_install (&ctrl->mot1, dir, "mot1");
    ACT_dc_motor_install (&ctrl->mot2, dir, "mot2");
    MFS_descriptor_create (dir, name, MFS_USR, &descriptor_op, (MFS_represent_t *) ctrl);
}
