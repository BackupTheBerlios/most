/*
 * motctrl.c
 *
 *  Created on: 31.07.2013
 *      Author: maik
 */


#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <act/2dc_motctrl.h>



extern void
ACT_2dc_mc_start(ACT_2dc_motctrl_t *ctrl)
{
    ctrl->cur_speed = 0;
    ACT_dc_motor_start(&ctrl->mot1);
    ACT_dc_motor_start(&ctrl->mot2);
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
ACT_2dc_mc_go(ACT_2dc_motctrl_t *ctrl, int speed)
{
    ctrl->cur_speed = check_speed(ctrl,speed);
    ACT_dc_motor_go(&ctrl->mot1, ctrl->cur_speed);
    ACT_dc_motor_go(&ctrl->mot2, ctrl->cur_speed);
}


extern void
ACT_2dc_mc_turn(ACT_2dc_motctrl_t *ctrl, int speed, int turn)
{
    int m1turn;
    int m2turn;
    ctrl->cur_speed = check_speed(ctrl, speed);
    m1turn = ctrl->cur_speed + turn;
    m2turn = ctrl->cur_speed - turn;
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
    ACT_dc_motor_init(&ctrl->mot1, ctrl->io->m1, ctrl->io->m1_dir_a, ctrl->io->m1_dir_b, max, dt, du, "motor1");
    ACT_dc_motor_init(&ctrl->mot2, ctrl->io->m2, ctrl->io->m2_dir_a, ctrl->io->m2_dir_b, max, dt, du, "motor2");
}

