/*
 * lfr.c
 *
 *  Created on: 05.08.2013
 *      Author: maik
 */

#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <uso/stack.h>
#include <uso/thread.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <cli/exec.h>
#include <mfs/directory.h>
#include <dev/adc.h>
#include <arch/pwm.h>
#include <arch/adc.h>
#include <arch/digio.h>
#include <lfr.h>
#include <encoder.h>
#include <usonic_ranger.h>
#include <act/2dc_motctrl.h>
#include <act/rc_servo.h>
#include <sen/linedetect.h>
#include <dpy/led_bar.h>


/*
 * LEFT ..... the line is left
 * RIGHT .... the line is right
 *
 *
 *                                          LOST  ----------------------------------------------
 *           .....................            ^              ................                  |
 *    |      .                   . |          |           |  .              .       |          |
 *  --|-------                -----|----------|-----------|-----            --------|-         |
 *    v      |                |    v    |     v      |    v    |            |       v          |
 *  RIGHT    |    RIGHT_LOST  |  RIGH   |   FOUND    |  LEFT   |  LEFT_LOST | LEFT             |
 *  SIDE     |                |         |            |         |            | SIDE             |
 *           |                |         |            |         |            |                  |
 *         <---             <---  OUT  <--          -->  OUT  -->          --->                |
 *           |                |    |    |            |    |    |            |                  |
 *           |                |    ------>          <------    |            |                  |
 *         -------------------->        |            |        <----------------                |
 *           |              --->  IN   -->          <--   IN  <--           |                  |
 *           |                |    |    |            |     |   |            |                  |
 *           |              <-------    |            |     ------>          |                  |
 *     |     |                |         |            |         |            |     |            |
 *  ...|......................|    ^    |      ^     |    ^    |..................|...         |
 *     |                      -----|-----------|----------|-----                  |            |
 *     v                           |           |          |                       v            |
 *                                                                                             |
 *                          |               SEARCH  <-------------------------------------------
 *                          |                    ^                                              
 * .........................|....................|..............................................
 *                          v                    |                                              
 *                         STOP   --------->   START                                            
 * 
 * 
 */


#define LFR_STACK_SIZE           (0x800/sizeof(USO_stack_t))

#define LFR_DRIVE(x)             ( ((x) * cfg.drive_amp) / cfg.drive_ab)
#define LFR_OUT(x)               ( ((x) * cfg.out_amp) / cfg.out_ab)
#define LFR_IN(x)                ( (-(x) * cfg.in_amp) / cfg.in_ab)
#define LFR_TURN_LEFT(x)         ((x))
#define LFR_TURN_RIGHT(x)        (-(x))



#define MOTOR_MAX_U              PWM_PERIOD_MOTOR   /* 100 */
#define MOTOR_SPEED_OFF           0
#define MOTOR_TURN_OFF            0
#define MOTOR_MIN_SPEED          15
#define MOTOR_MAX_SPEED         100
#define MOTOR_MIN_TURN           15
#define MOTOR_MAX_TURN          100



#define SERVO_DUTY_MIN           16
#define SERVO_DUTY_MAX           70
#define SERVO_ANGLE              90

#define LINE_SENSOR_DIV          (LA2_ADC0_RESOLUTION/MOTOR_MAX_U)




enum LFR_state {
    LFR_STOP,
    LFR_START,
    LFR_SEARCH,
    LFR_DRIVE,
    LFR_LEFT_OUT,
    LFR_RIGHT_OUT,
    LFR_LEFT_IN,
    LFR_RIGHT_IN,
    LFR_LEFT_LOST,
    LFR_RIGHT_LOST,
    LFR_LOST,
    LFR_LEFT_SIDE,
    LFR_RIGHT_SIDE
};


struct LFR_cfg{
    int interval_ms;
    int drive_pulse_interval;
    int drive_pulse;
    int out_pulse_interval;
    int out_pulse;
    int in_pulse;
    int lost_pulse;
    int side_pulse;
    int drive_amp;
    int drive_ab;
    int out_amp;
    int out_ab;
    int in_amp;
    int in_ab;
    int dav_lost;
    int search_counter;
    int lost_counter;
    int in_counter;
    int side_counter;
    int interval_counter;
    int speed_inc;
    int speed_dec;
    int speed_low;
    int speed_full;
    int turn_low;
    int turn_med;
    int turn_hi;
    int turn_full;
    int motor_dt;
    int motor_du;
    int line_threshold;
};

typedef struct LFR_cfg LFR_cfg_t;

static LFR_cfg_t cfg = {
    .interval_ms          = 2, /* 5ms -> v = 1 m/s -> resolution = 5mm, v = 0,1 m/s ->resolution = 0,5mm */
    .drive_pulse_interval = 5,
    .drive_pulse          = 2,
    .out_pulse_interval   = 4,
    .out_pulse            = 10,
    .in_pulse             = 10,
    .lost_pulse           = 40,
    .side_pulse           = 40,
    .drive_amp            = 1,
    .drive_ab             = 1,
    .out_amp              = 1,
    .out_ab               = 2,
    .in_amp               = 1,
    .in_ab                = 1,
    .dav_lost             = 50,
    .search_counter       = 3000,     /* how long we accept the state for searching the line */ 
    .lost_counter         = 500,      /* how long we accept the state with no line */ 
    .in_counter           = 30,       /* how long we accept the state for coming into the line (breaking) */ 
    .side_counter         = 1000,     /* how long we accept the state knowing the line is on the left side */ 
    .interval_counter     = 25,     /* I counter * I time * 2 = time for displaying values and measure distance. 100ms */
    .speed_inc            = 5,
    .speed_dec            = 5,
    .speed_low            = 20,
    .speed_full           = 35,
    .turn_low             = 15,
    .turn_med             = 20,
    .turn_hi              = 30,
    .turn_full            = 50,
    /* Bsp.: MAX_U=100->10V, du=1->0,1V, dt=1 => du/dt = 0,1V/ms -> delta u = 10V (0-MAX)= 100ms -> (v = 1m/s) ds = 10cm */
    .motor_dt             = 2,    /* 1 - LFR_INTERVAL_MS: ms to inc/dec U +/-du */
    .motor_du             = 20,     /* 1 - MOTOR_MAX_U */
    .line_threshold       = 25  /* 0 - MOTOR_MAX_U : 0 = full reflection (no line), MOTOR_MAX_U = no reflection (perfect line)*/
};


static struct ACT_2dc_motctrl_io motctrl_io;

static ACT_2dc_motctrl_t mc;
static ACT_rc_servo_t servo;
static SEN_linedetect_t ld;
static DPY_led_bar_t motor_display;
static DPY_led_bar_t line_display;

static enum LFR_state state;
static enum SEN_line_state line;

static int diff;
static int ddiff;
static int av;
static int dav;
static int av_sum;
static int speed;
static int speed_sum;
static int turn;
static int turn_sum;
static int state_counter;
static ACE_bool_t display_toggle;
static int interval_cnt;
static int distance_cm;


static void
set_line_display(int line_val)
{
    int red = 0, yellow = 0;
    int green = (line_val * LED_BAR_GREEN_MAX) / MOTOR_MAX_U;
    if (green > LED_BAR_GREEN_MAX) green = LED_BAR_GREEN_MAX;
    switch (line){
        case SEN_LINE_FOUND:
            break;
        case SEN_LINE_LEFT:
            red = 1;
            break;
        case SEN_LINE_RIGHT:
            yellow = 1;
            break;
        case SEN_LINE_LOST:
        default:
            red = 1;
            yellow = 1;
            break;
    }
    DPY_led_bar_set(&line_display, green, yellow, red);
}

static void
set_motor_display(int turn_val, int speed_val)
{
    int red = 0, yellow = 0;
    if (turn_val > 0){
        red = 1;
    }
    else if (turn_val < 0){
        yellow = 1;
    }
    if (speed_val < 0){
        red = 1;
        yellow = 1;
        speed_val = -speed_val;
    }
    int green = (speed_val * LED_BAR_GREEN_MAX) / MOTOR_MAX_U;
    if (green > LED_BAR_GREEN_MAX) green = LED_BAR_GREEN_MAX;
    DPY_led_bar_set(&motor_display, green, yellow, red);
}

static void
init (void)
{
    speed = MOTOR_SPEED_OFF;
    turn = MOTOR_TURN_OFF;
    diff = 0;
    ddiff = 0;
    av = 0;
    dav = 0;
    av_sum = 0;
    speed_sum = 0;
    turn_sum = 0;
    state_counter = 0;
    display_toggle = FALSE;
    interval_cnt = 0;
    ACT_rc_servo_set_pos (&servo, 0);
    distance_cm = SEN_usonic_ranger_measure_cm((SEN_usonic_ranger_t *)&usonic);
    distance_cm = SEN_usonic_ranger_measure_cm((SEN_usonic_ranger_t *)&usonic);  /* the first value is not valid */
}

static void
stop (void) {
    init();
    ACT_2dc_mc_stop(&mc);
    USO_sleep(USO_MSEC_2_TICKS(3000));
    state = LFR_START;
}

static void
start (void) {
    if (DEV_digin_isset(&LA2_pushbutton) == FALSE){
        return;
    }
    LA2_beep();
    ACT_2dc_mc_start(&mc);
    USO_sleep(USO_MSEC_2_TICKS(3000));
    state = LFR_SEARCH;
}

static void
search (void) {
    speed = MOTOR_SPEED_OFF;
    if (state_counter <= (cfg.search_counter / 3)){
        turn = LFR_TURN_RIGHT(cfg.turn_hi);
    } else {
        turn = LFR_TURN_LEFT(cfg.turn_hi);
    }
}

static void
drive (void) {
    if (speed < cfg.speed_full){
        speed += cfg.speed_inc;
    }
    if (ld.mode == SEN_LINE_ANALOG){
        if (state_counter <= cfg.drive_pulse){
            turn = LFR_DRIVE(diff + ddiff);
        } else {
            turn = MOTOR_TURN_OFF;
        }
        if (state_counter >= (cfg.drive_pulse_interval * cfg.drive_pulse)){
            state_counter = 0;
        }
    } else { 
        turn = MOTOR_TURN_OFF;
    }
}

static void
left_out (void) {
    int t;
    if (ld.mode == SEN_LINE_ANALOG){
        t = LFR_OUT(diff + ddiff);
    } else {
        turn = LFR_TURN_LEFT(cfg.turn_hi);
    }
    if (state_counter <= cfg.out_pulse){
        turn = t;
    } else {
        turn = LFR_TURN_LEFT(cfg.turn_low);
    }
    if (state_counter >= (cfg.out_pulse_interval * cfg.out_pulse)){
        state_counter = 0;
    }
}

static void
right_out (void) {
    int t;
    if (ld.mode == SEN_LINE_ANALOG){
        t = turn = LFR_OUT(diff + ddiff);
    } else {
        t = LFR_TURN_RIGHT(cfg.turn_hi);
    }
    if (state_counter <= cfg.out_pulse){
        turn = t;
    } else {
        turn = LFR_TURN_RIGHT(cfg.turn_low);
    }
    if (state_counter >= (cfg.out_pulse_interval * cfg.out_pulse)){
        state_counter = 0;
    }
}

static void
left_in (void) {
    int t;
    if (ld.mode == SEN_LINE_ANALOG){
        t = LFR_IN(diff + ddiff);
    } else {
        t = LFR_TURN_LEFT(-cfg.turn_med);
    }
    if (state_counter <= cfg.in_pulse){
        turn = t;
    } else {
        turn = MOTOR_TURN_OFF;
    }
}

static void
right_in (void) {
    int t;
    if (ld.mode == SEN_LINE_ANALOG){
        t = LFR_IN(diff + ddiff);
    } else {
        t = LFR_TURN_RIGHT(-cfg.turn_med);
    }
    if (state_counter <= cfg.in_pulse){
        turn = t;
    } else {
        turn = MOTOR_TURN_OFF;
    }
}

static void
left_lost (void) {
    if (state_counter <= cfg.lost_pulse){
        turn = LFR_TURN_LEFT(cfg.turn_full);
    } else {
        turn = LFR_TURN_LEFT(cfg.turn_low);
    }
}

static void
right_lost (void) {
    if (state_counter <= cfg.lost_pulse){
        turn = LFR_TURN_RIGHT(cfg.turn_full);
    } else {
        turn = LFR_TURN_RIGHT(cfg.turn_low);
    }
}

static void
lost (void) {
    turn = MOTOR_TURN_OFF;
}

static void
left_side (void) {
    if (speed > -cfg.speed_low) {
        speed -= cfg.speed_dec;
    }
    if (state_counter <= cfg.side_pulse){
        turn = LFR_TURN_LEFT(cfg.turn_full);
    } else {
        turn = LFR_TURN_LEFT(cfg.turn_low);
    }
}

static void
right_side (void) {
    if (speed > -cfg.speed_low) {
        speed -= cfg.speed_dec;
    }
    if (state_counter <= cfg.side_pulse){
        turn = LFR_TURN_RIGHT(cfg.turn_full);
    } else {
        turn = LFR_TURN_RIGHT(cfg.turn_low);
    }
}

static void
state_action(void)
{
    switch (state){
        case LFR_STOP:
            stop();
            break;
        case LFR_START:
            start();
            break;
        case LFR_SEARCH:
            search();
            break;
        case LFR_DRIVE:
            drive();
            break;
        case LFR_LEFT_OUT:
            left_out();
            break;
        case LFR_RIGHT_OUT:
            right_out();
            break;
        case LFR_LEFT_IN:
            left_in();
            break;
        case LFR_RIGHT_IN:
            right_in();
            break;
        case LFR_LEFT_LOST:
            left_lost();
            break;
        case LFR_RIGHT_LOST:
            right_lost();
            break;
        case LFR_LOST:
            lost();
            break;
        case LFR_LEFT_SIDE:
            left_side();
            break;
        case LFR_RIGHT_SIDE:
            right_side();
            break;
        default:
            break;
    }
    state_counter++;
}



static void
search_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_IN;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_IN;
            break;
        case SEN_LINE_LOST:
            if (state_counter >= cfg.search_counter){
                state = LFR_STOP;
            }
            break;
        default:
            break;
    }
}

static void
drive_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_OUT;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_OUT;
            break;
        case SEN_LINE_LOST:
            state = LFR_LOST;
            break;
        default:
            break;
    }
}

static void
left_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            if (state_counter >= cfg.in_counter){
                state = LFR_LEFT_OUT;
            }
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_OUT;
            break;
        case SEN_LINE_LOST:
            if (dav >= cfg.dav_lost){
                state = LFR_LOST;
            } else {
                state = LFR_LEFT_LOST;
            }
            break;
        default:
            break;
    }
}

static void
right_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_OUT;
            break;
        case SEN_LINE_RIGHT:
            if (state_counter >= cfg.in_counter){
                state = LFR_RIGHT_OUT;
            }
            break;
        case SEN_LINE_LOST:
            if (dav >= cfg.dav_lost){
                state = LFR_LOST;
            } else {
                state = LFR_RIGHT_LOST;
            }
            break;
        default:
            break;
    }
}

static void
left_lost_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_IN;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_OUT;
            break;
        case SEN_LINE_LOST:
            if (DEV_digin_isset(&LA2_side_line_1) == FALSE){
                state = LFR_LEFT_SIDE;
            } else if ((DEV_digin_isset(&LA2_side_line_2) == FALSE)){
                state = LFR_RIGHT_SIDE;
            } else if (state_counter >= cfg.lost_counter){
                state = LFR_LEFT_SIDE;
            }
            break;
        default:
            break;
    }
}

static void
right_lost_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_OUT;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_IN;
            break;
        case SEN_LINE_LOST:
            if (DEV_digin_isset(&LA2_side_line_2) == FALSE){
                state = LFR_RIGHT_SIDE;
            } else if ((DEV_digin_isset(&LA2_side_line_1) == FALSE)){
                state = LFR_LEFT_SIDE;
            } else if (state_counter >= cfg.lost_counter){
                state = LFR_RIGHT_SIDE;
            }
            break;
        default:
            break;
    }
}

static void
lost_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_OUT;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_OUT;
            break;
        case SEN_LINE_LOST:
            if ( (DEV_digin_isset(&LA2_side_line_1) == FALSE) && 
                 (DEV_digin_isset(&LA2_side_line_2) == TRUE) ){
                state = LFR_LEFT_SIDE;
            } else if ( (DEV_digin_isset(&LA2_side_line_2) == FALSE) &&
                        (DEV_digin_isset(&LA2_side_line_1) == TRUE) ){
                state = LFR_RIGHT_SIDE;
            } else if (state_counter >= cfg.lost_counter){
                state = LFR_SEARCH;
            }
            break;
        default:
            break;
    }
}

static void
left_side_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_IN;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_OUT;
            break;
        case SEN_LINE_LOST:
            if (state_counter >= cfg.side_counter){
                state = LFR_SEARCH;
            }
            break;
        default:
            break;
    }
}

static void
right_side_trans (void) {
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT_OUT;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT_IN;
            break;
        case SEN_LINE_LOST:
            if (state_counter >= cfg.side_counter){
                state = LFR_SEARCH;
            }
            break;
        default:
            break;
    }
}

static void
change_state(void)
{
    enum LFR_state old = state;
    switch (state){
        case LFR_STOP:
        case LFR_START:
            break;
        case LFR_SEARCH:
            search_trans();
            break;
        case LFR_DRIVE:
            drive_trans();
            break;
        case LFR_LEFT_OUT:
        case LFR_LEFT_IN:
            left_trans();
            break;
        case LFR_RIGHT_OUT:
        case LFR_RIGHT_IN:
            right_trans();
            break;
        case LFR_LEFT_LOST:
            left_lost_trans();
            break;
        case LFR_RIGHT_LOST:
            right_lost_trans();
            break;
        case LFR_LOST:
            lost_trans();
            break;
        case LFR_LEFT_SIDE:
            left_side_trans();
            break;
        case LFR_RIGHT_SIDE:
            right_side_trans();
            break;
        default:
            break;
    }
    if (old != state){
        state_counter = 0;
    }
}

/* this function is maybe called in the context of an interrupt */
static void
lfr_cleanup (void *arg)
{
    stop();
    ACT_rc_servo_set_pos (&servo, 0);
    ACT_rc_servo_disable(&servo);
    LFR_encoder_disable(&enc1);
    LFR_encoder_disable(&enc1);
    LFR_ultra_sonic_disable(&usonic);
}

static ACE_err_t
lfr_run (void *arg)
{
    USO_log_puts (USO_LL_INFO, "LFR running.\n");
    ACT_rc_servo_enable(&servo);
    LFR_encoder_enable(&enc1);
    LFR_encoder_enable(&enc1);
    LFR_ultra_sonic_enable(&usonic);
    USO_cleanup_install (lfr_cleanup);
    init();
    state = LFR_START;
    for (;;){
        line = SEN_line_get_state(&ld);
        diff = SEN_line_get_diff(&ld);
        ddiff = ACE_AMOUNT(SEN_line_get_ddiff(&ld));
        av = SEN_line_get_av(&ld);
        dav = SEN_line_get_dav(&ld);
        change_state();
        state_action();
        if (state != LFR_START && state != LFR_STOP){
            if (distance_cm < 30) {
                state = LFR_STOP;
                continue;
            }
            ACT_2dc_mc_go (&mc, speed, turn);
        }
        av_sum += av;
        speed_sum += speed;
        turn_sum += turn;
        if (interval_cnt++ >= cfg.interval_counter) {
            if (display_toggle == FALSE){
                SEN_usonic_ranger_measure_start((SEN_usonic_ranger_t *)&usonic);
                set_line_display(av_sum / (cfg.interval_counter * 2));
                av_sum = 0;
                display_toggle = TRUE;
            } else {    
                distance_cm = SEN_usonic_ranger_get_cm((SEN_usonic_ranger_t *)&usonic);
                ACT_rc_servo_set_pos (&servo, 0);
                set_motor_display(turn_sum, speed_sum / (cfg.interval_counter * 2));
                speed_sum = 0;
                turn_sum = 0;
                display_toggle = FALSE;
            }
            interval_cnt = 0;
        }
        USO_sleep(USO_MSEC_2_TICKS(cfg.interval_ms));
    }
    return DEF_ERR_SYS;
}


extern void LFR_init (void)
{
    motctrl_io.m1 = &LA2_motor1;
    motctrl_io.m2 = &LA2_motor2;
    motctrl_io.m1_dir_a = &LA2_m1_a;
    motctrl_io.m1_dir_b = &LA2_m1_b;
    motctrl_io.m2_dir_a = &LA2_m2_a;
    motctrl_io.m2_dir_b = &LA2_m2_b;
    DPY_led_bar_init(&line_display, &LA2_led_bar1_clk, &LA2_led_bar1_data, NULL);
    DPY_led_bar_init(&motor_display, &LA2_led_bar2_clk, &LA2_led_bar2_data, NULL);
    SEN_line_init(&ld, &adc0, DEV_ADC_CHN_0, DEV_ADC_CHN_1, &LA2_line_1, &LA2_line_2,
                  LINE_SENSOR_DIV, cfg.line_threshold, MOTOR_MAX_U, SEN_LINE_ANALOG);
    ACT_2dc_mc_init (&mc, &motctrl_io, MOTOR_MAX_U, cfg.motor_dt, cfg.motor_du);
    ACT_rc_servo_init(&servo, &LA2_servo, SERVO_ANGLE, PWM_PERIOD_SERVO, SERVO_DUTY_MIN, SERVO_DUTY_MAX);
    LFR_encoder_init();
    LFR_ultra_sonic_init();
}



static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    LFR_cfg_t *lfr = (LFR_cfg_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "I_ms";
            entry->value.l = lfr->interval_ms;
            break;
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "D_puls_I";
            entry->value.l = lfr->drive_pulse_interval;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "D_puls";
            entry->value.l = lfr->drive_pulse;
            break;
        case 3:
            entry->type = MFS_INFO_LONG;
            entry->name = "OUT_puls_I";
            entry->value.l = lfr->out_pulse_interval;
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "OUT_puls";
            entry->value.l = lfr->out_pulse;
            break;
        case 5:
            entry->type = MFS_INFO_LONG;
            entry->name = "IN_puls";
            entry->value.l = lfr->in_pulse;
            break;
        case 6:
            entry->type = MFS_INFO_LONG;
            entry->name = "L_puls";
            entry->value.l = lfr->lost_pulse;
            break;
        case 7:
            entry->type = MFS_INFO_LONG;
            entry->name = "S_puls";
            entry->value.l = lfr->side_pulse;
            break;
        case 8:
            entry->type = MFS_INFO_LONG;
            entry->name = "D_amp";
            entry->value.l = lfr->drive_amp;
            break;
        case 9:
            entry->type = MFS_INFO_LONG;
            entry->name = "D_ab";
            entry->value.l = lfr->drive_ab;
            break;
        case 10:
            entry->type = MFS_INFO_LONG;
            entry->name = "OUT_amp";
            entry->value.l = lfr->out_amp;
            break;
        case 11:
            entry->type = MFS_INFO_LONG;
            entry->name = "OUT_ab";
            entry->value.l = lfr->out_ab;
            break;
        case 12:
            entry->type = MFS_INFO_LONG;
            entry->name = "IN_amp";
            entry->value.l = lfr->in_amp;
            break;
        case 13:
            entry->type = MFS_INFO_LONG;
            entry->name = "IN_ab";
            entry->value.l = lfr->in_ab;
            break;
        case 14:
            entry->type = MFS_INFO_LONG;
            entry->name = "DAV_lost";
            entry->value.l = lfr->dav_lost;
            break;
        case 15:
            entry->type = MFS_INFO_LONG;
            entry->name = "Search_cnt";
            entry->value.l = lfr->search_counter;
            break;
        case 16:
            entry->type = MFS_INFO_LONG;
            entry->name = "Lost_cnt";
            entry->value.l = lfr->lost_counter;
            break;
        case 17:
            entry->type = MFS_INFO_LONG;
            entry->name = "In_cnt";
            entry->value.l = lfr->in_counter;
            break;
        case 18:
            entry->type = MFS_INFO_LONG;
            entry->name = "Side_cnt";
            entry->value.l = lfr->side_counter;
            break;
        case 19:
            entry->type = MFS_INFO_LONG;
            entry->name = "Interval_cnt";
            entry->value.l = lfr->interval_counter;
            break;
        case 20:
            entry->type = MFS_INFO_LONG;
            entry->name = "Speed_inc";
            entry->value.l = lfr->speed_inc;
            break;
        case 21:
            entry->type = MFS_INFO_LONG;
            entry->name = "Speed_dec";
            entry->value.l = lfr->speed_dec;
            break;
        case 22:
            entry->type = MFS_INFO_LONG;
            entry->name = "Speed_low";
            entry->value.l = lfr->speed_low;
            break;
        case 23:
            entry->type = MFS_INFO_LONG;
            entry->name = "Speed_full";
            entry->value.l = lfr->speed_full;
            break;
        case 24:
            entry->type = MFS_INFO_LONG;
            entry->name = "Turn_low";
            entry->value.l = lfr->turn_low;
            break;
        case 25:
            entry->type = MFS_INFO_LONG;
            entry->name = "Turn_med";
            entry->value.l = lfr->turn_med;
            break;
        case 26:
            entry->type = MFS_INFO_LONG;
            entry->name = "Turn_hi";
            entry->value.l = lfr->turn_hi;
            break;
        case 27:
            entry->type = MFS_INFO_LONG;
            entry->name = "Turn_full";
            entry->value.l = lfr->turn_full;
            break;
        case 28:
            entry->type = MFS_INFO_LONG;
            entry->name = "Motor_dt";
            entry->value.l = lfr->motor_dt;
            break;
        case 29:
            entry->type = MFS_INFO_LONG;
            entry->name = "Motor_du";
            entry->value.l = lfr->motor_du;
            break;
        case 30:
            entry->type = MFS_INFO_LONG;
            entry->name = "Line_threshold";
            entry->value.l = lfr->line_threshold;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}



static void
control (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    LFR_cfg_t *lfr = (LFR_cfg_t *) desc->represent;
    if (entry->type == MFS_CTRL_HELP){
        ACE_sprintf(entry->value.s, "\t n l same as info\n");
        return;
    }
    switch (number){
        case 0:
            if (entry->type == MFS_CTRL_LONG){
                lfr->interval_ms = entry->value.l;
            }
            break;
        case 1:
            if (entry->type == MFS_CTRL_LONG){
                lfr->drive_pulse_interval = entry->value.l;
            }
            break;
        case 2:
            if (entry->type == MFS_CTRL_LONG){
                lfr->drive_pulse = entry->value.l;
            }
            break;
        case 3:
            if (entry->type == MFS_CTRL_LONG){
                lfr->out_pulse_interval = entry->value.l;
            }
            break;
        case 4:
            if (entry->type == MFS_CTRL_LONG){
                lfr->out_pulse = entry->value.l;
            }
            break;
        case 5:
            if (entry->type == MFS_CTRL_LONG){
                lfr->in_pulse = entry->value.l;
            }
            break;
        case 6:
            if (entry->type == MFS_CTRL_LONG){
                lfr->lost_pulse = entry->value.l;
            }
            break;
        case 7:
            if (entry->type == MFS_CTRL_LONG){
                lfr->side_pulse = entry->value.l;
            }
            break;
        case 8:
            if (entry->type == MFS_CTRL_LONG){
                lfr->drive_amp = entry->value.l;
            }
            break;
        case 9:
            if (entry->type == MFS_CTRL_LONG){
                lfr->drive_ab = entry->value.l;
            }
            break;
        case 10:
            if (entry->type == MFS_CTRL_LONG){
                lfr->out_amp = entry->value.l;
            }
            break;
        case 11:
            if (entry->type == MFS_CTRL_LONG){
                lfr->out_ab = entry->value.l;
            }
            break;
        case 12:
            if (entry->type == MFS_CTRL_LONG){
                lfr->in_amp = entry->value.l;
            }
            break;
        case 13:
            if (entry->type == MFS_CTRL_LONG){
                lfr->in_ab = entry->value.l;
            }
            break;
        case 14:
            if (entry->type == MFS_CTRL_LONG){
                lfr->dav_lost = entry->value.l;
            }
            break;
        case 15:
            if (entry->type == MFS_CTRL_LONG){
                lfr->search_counter = entry->value.l;
            }
            break;
        case 16:
            if (entry->type == MFS_CTRL_LONG){
                lfr->lost_counter = entry->value.l;
            }
            break;
        case 17:
            if (entry->type == MFS_CTRL_LONG){
                lfr->in_counter = entry->value.l;
            }
            break;
        case 18:
            if (entry->type == MFS_CTRL_LONG){
                lfr->side_counter = entry->value.l;
            }
            break;
        case 19:
            if (entry->type == MFS_CTRL_LONG){
                lfr->interval_counter = entry->value.l;
            }
            break;
        case 20:
            if (entry->type == MFS_CTRL_LONG){
                lfr->speed_inc = entry->value.l;
            }
            break;
        case 21:
            if (entry->type == MFS_CTRL_LONG){
                lfr->speed_dec = entry->value.l;
            }
            break;
        case 22:
            if (entry->type == MFS_CTRL_LONG){
                lfr->speed_low = entry->value.l;
            }
            break;
        case 23:
            if (entry->type == MFS_CTRL_LONG){
                lfr->speed_full = entry->value.l;
            }
            break;
        case 24:
            if (entry->type == MFS_CTRL_LONG){
                lfr->turn_low = entry->value.l;
            }
            break;
        case 25:
            if (entry->type == MFS_CTRL_LONG){
                lfr->turn_med = entry->value.l;
            }
            break;
        case 26:
            if (entry->type == MFS_CTRL_LONG){
                lfr->turn_hi = entry->value.l;
            }
            break;
        case 27:
            if (entry->type == MFS_CTRL_LONG){
                lfr->turn_full = entry->value.l;
            }
            break;
        case 28:
            if (entry->type == MFS_CTRL_LONG){
                lfr->motor_dt = entry->value.l;
            }
            break;
        case 29:
            if (entry->type == MFS_CTRL_LONG){
                lfr->motor_du = entry->value.l;
            }
            break;
        case 30:
            if (entry->type == MFS_CTRL_LONG){
                lfr->line_threshold = entry->value.l;
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


extern void LFR_install(MFS_descriptor_t *dir, char *name)
{
    SEN_line_install (&ld, dir, "line");
    ACT_2dc_mc_install(&mc, dir, "motctrl");
    ACT_rc_servo_install(&servo, dir, "servo");
    SEN_encoder_install ((SEN_encoder_t *)&enc1, dir, "enc1");
    SEN_encoder_install ((SEN_encoder_t *)&enc2, dir, "enc2");
    SEN_usonic_ranger_install ((SEN_usonic_ranger_t *)&usonic, dir, "usonic");
    MFS_descriptor_create (dir, name, MFS_USR, &descriptor_op, (MFS_represent_t *) &cfg);
}

extern void
LFR_start (void)
{
    USO_thread_t *t = USO_thread_new (lfr_run, LFR_STACK_SIZE, USO_SYSTEM, USO_FIFO, "lfr");
    if (t != NULL){
        USO_start(t);
    }
}
