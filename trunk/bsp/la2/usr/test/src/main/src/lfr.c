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
#include <act/2dc_motctrl.h>
#include <sen/linedetect.h>
#include <dpy/led_bar.h>


#define LFR_STACK_SIZE           (0x800/sizeof(USO_stack_t))
#define LFR_INTERVAL_MS          10   /* 5ms -> v = 1 m/s -> resolution = 5mm, v = 0,1 m/s ->resolution = 0,5mm */
#define LFR_TURN_ABSORPTION      3
#define LFR_TURN_AMPLIFICATION   2
#define LFR_TURN_CALC(x)         ((x*LFR_TURN_AMPLIFICATION)/LFR_TURN_ABSORPTION)
#define LFR_TURN_LEFT(x)         (-LFR_TURN_CALC(x))
#define LFR_TURN_RIGHT(x)        (LFR_TURN_CALC(x))
#define LFR_TURN_DIFF(x)         (LFR_TURN_CALC(x))

#define MOTOR_MAX_U              PWM_PERIOD_MOTOR
#define MOTOR_LOW_SPEED          30
#define MOTOR_MED_SPEED          40
#define MOTOR_HI_SPEED           60
#define MOTOR_FULL_SPEED         80
#define MOTOR_TURN_SPEED         40    /* turn speed for digital sensors */
/* Bsp.: MAX_U=100->10V, du=1->0,1V, dt=1 => du/dt = 0,1V/ms -> delta u = 10V (0-MAX)= 100ms -> (v = 1m/s) ds = 10cm */
#define MOTOR_DT                 2     /* 1 - LFR_INTERVAL_MS: ms to inc/dec U +/-du */
#define MOTOR_DU                 5     /* 1 - MOTOR_MAX_U */

#define LINE_SENSOR_THRESHOLD    25  /* 0 - MOTOR_MAX_U : 0 = full reflection (no line), MOTOR_MAX = no reflection (perfect line)*/
#define LINE_SENSOR_DIV          (LA2_ADC0_RESOLUTION/MOTOR_MAX_U)

#define DISPLAY_INTERVAL_CNT     5


enum LFR_state {
    LFR_STOP,
    LFR_START,
    LFR_SEARCH,
    LFR_DRIVE,
    LFR_LEFT,
    LFR_RIGHT,
    LFR_LOST
};


static struct ACT_2dc_motctrl_io motctrl_io;

static ACT_2dc_motctrl_t mc;
static SEN_linedetect_t ld;
static DPY_led_bar_t motor_display;
static DPY_led_bar_t line_display;
static enum LFR_state state;
static enum SEN_line_state line;
static int diff;
static int ddiff;
static int av;
static int speed;
static int turn;
static int max;
static ACE_bool_t calc_speed;
static ACE_bool_t calc_turn;

static CLI_exec_t mstart;
static CLI_exec_t mstop;
static CLI_exec_t mgo;
static CLI_exec_t mturn;

static void
motor_off(void)
{
    speed = 0;
    turn = 0;
    max = 0;
    calc_speed = FALSE;
    calc_turn = FALSE;
}

/* this function is maybe called in the context of an interrupt */
static void
lfr_cleanup (void *arg)
{
    motor_off();
    ACT_2dc_mc_stop(&mc);
    state = LFR_STOP;
}

static void
stop (void) {
    motor_off();
    ACT_2dc_mc_start(&mc);
    state = LFR_START;
}

static void
start (void) {
    motor_off();
    if (DEV_digin_isset(&LA2_pushbutton) == FALSE){
        return;
    }
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_SEARCH;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_SEARCH;
            break;
        case SEN_LINE_LOST:
            state = LFR_SEARCH;
            break;
        default:
            break;
    }
    LA2_beep();
    USO_sleep(USO_MSEC_2_TICKS(3000));
}

static void
search (void) {
    motor_off();
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            turn = 0;
            break;
        case SEN_LINE_LEFT:
            turn = LFR_TURN_LEFT(MOTOR_LOW_SPEED);
            break;
        case SEN_LINE_RIGHT:
            turn = LFR_TURN_RIGHT(MOTOR_LOW_SPEED);
            break;
        case SEN_LINE_LOST:
            turn = LFR_TURN_RIGHT(MOTOR_MED_SPEED);
            break;
        default:
            break;
    }
}

static void
drive (void) {
    max = MOTOR_HI_SPEED;
    switch (line){
        case SEN_LINE_FOUND:
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT;
            break;
        case SEN_LINE_LOST:
            state = LFR_LOST;
            motor_off();
            speed = MOTOR_LOW_SPEED;
            turn = 0;
            break;
        default:
            motor_off();
            break;
    }
}

static void
left (void) {
    max = MOTOR_HI_SPEED;
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT;
            max = MOTOR_MED_SPEED;
            break;
        case SEN_LINE_LOST:
            state = LFR_LOST;
            motor_off();
            speed = MOTOR_LOW_SPEED;
            turn = LFR_TURN_LEFT(MOTOR_MED_SPEED);
            break;
        default:
            motor_off();
            break;
    }
}

static void
right (void) {
    max = MOTOR_HI_SPEED;
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT;
            max = MOTOR_MED_SPEED;
            break;
        case SEN_LINE_RIGHT:
            break;
        case SEN_LINE_LOST:
            state = LFR_LOST;
            motor_off();
            speed = MOTOR_LOW_SPEED;
            turn = LFR_TURN_RIGHT(MOTOR_MED_SPEED);
            break;
        default:
            motor_off();
            break;
    }
}

static void
lost (void) {
    max = MOTOR_MED_SPEED;
    switch (line){
        case SEN_LINE_FOUND:
            state = LFR_DRIVE;
            break;
        case SEN_LINE_LEFT:
            state = LFR_LEFT;
            break;
        case SEN_LINE_RIGHT:
            state = LFR_RIGHT;
            break;
        case SEN_LINE_LOST:
            state = LFR_SEARCH;
            motor_off();
            break;
        default:
            motor_off();
            break;
    }
}

static void
set_line_display(void)
{
    int red = 0, yellow = 0;
    int green_range = (av * 8) / MOTOR_MAX_U;
    switch (line){
        case SEN_LINE_FOUND:
            break;
        case SEN_LINE_LEFT:
            yellow = 1;
            break;
        case SEN_LINE_RIGHT:
            yellow = 1;
            break;
        case SEN_LINE_LOST:
            red = 1;
            break;
        default:
            red = 1;
            yellow = 1;
            break;
    }
    DPY_led_bar_set(&line_display, green_range, yellow, red);
}

static void
set_motor_display(void)
{
    int red = 0, yellow = 0;
    if (turn > 0)
        yellow = 1;
    else if (turn < 0)
        red = 1;
    int green_range = (ACE_AMOUNT(speed) * 8) / MOTOR_MAX_U;
    DPY_led_bar_set(&motor_display, green_range, yellow, red);
}

static void
lfr_run (void *arg)
{
    USO_log_puts (USO_LL_INFO, "LFR running.\n");
    USO_cleanup_install (lfr_cleanup);
    state = LFR_STOP;
    motor_off();
    int cnt = 0;
    for (;;){
        if (++cnt > DISPLAY_INTERVAL_CNT) cnt = 0; 
        line = SEN_line_get_state(&ld);
        diff = SEN_line_get_diff(&ld);
        ddiff = SEN_line_get_ddiff(&ld);
        av = SEN_line_get_av(&ld);
        if (cnt == 0) set_line_display();
        calc_speed = TRUE;
        calc_turn = TRUE;
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
            case LFR_LEFT:
                left();
                break;
            case LFR_RIGHT:
                right();
                break;
            case LFR_LOST:
                lost();
                break;
            default:
                break;
        }
        if (ld.mode == SEN_LINE_ANALOG){
            if (calc_speed){
                speed = max - ( (max * ddiff)/MOTOR_MAX_U);
            }
            if (calc_turn){
                turn = LFR_TURN_DIFF(diff);
            }
        } else {
            if (calc_speed){
                speed = max;
            }
            if (calc_turn){
                turn = (LFR_TURN_DIFF(diff) * MOTOR_TURN_SPEED) / MOTOR_MAX_U;
            }
        }
        if (state != LFR_STOP){
            ACT_2dc_mc_turn (&mc, speed, turn);
            if (cnt == 0) set_motor_display();
        }
        USO_sleep(USO_MSEC_2_TICKS(LFR_INTERVAL_MS));
    }
}


extern void LFR_init (void)
{
    motctrl_io.m1 = &LA2_motor1;
    motctrl_io.m2 = &LA2_motor2;
    motctrl_io.m1_dir_a = &LA2_m1_a;
    motctrl_io.m1_dir_b = &LA2_m1_b;
    motctrl_io.m2_dir_a = &LA2_m2_a;
    motctrl_io.m2_dir_b = &LA2_m2_b;
    DPY_led_bar_init(&line_display, &LA2_led_bar1_clk, &LA2_led_bar1_data);
    DPY_led_bar_init(&motor_display, &LA2_led_bar2_clk, &LA2_led_bar2_data);
    SEN_line_init(&ld, &adc0, DEV_ADC_CHN_0, DEV_ADC_CHN_1, &LA2_line_1, &LA2_line_2,
                  LINE_SENSOR_DIV, LINE_SENSOR_THRESHOLD, MOTOR_MAX_U, SEN_LINE_ANALOG);
    ACT_2dc_mc_init (&mc, &motctrl_io, MOTOR_MAX_U, MOTOR_DT, MOTOR_DU);
    USO_thread_new (lfr_run, LFR_STACK_SIZE, USO_SYSTEM, USO_FIFO, "lfr");
}

static int
parse (char* arg){
    int u = 0;
    u = ACE_atoi (&arg[1]);
    switch (arg[0]){
        case '+':
            break;
        case '-':
            u = -u;
            break;
        default:
            u = 0;
            break;
    }
    return u;
}

static void
mstart_exec (char *nix)
{
    ACT_2dc_mc_start(&mc);
}

static void
mstop_exec (char *nix)
{
    ACT_2dc_mc_stop(&mc);
}

static void
mgo_exec (char *arg)
{
    if (arg != NULL) {
        ACT_2dc_mc_go(&mc, parse(arg));
    }
}

static void
mturn_exec (char *arg)
{
    if (arg != NULL) {
        ACT_2dc_mc_turn(&mc, mc.cur_speed, parse(arg));
    }
}

extern void LFR_install(MFS_descriptor_t *dir)
{
    MFS_descriptor_t *lfr;
    lfr = MFS_directory_create (dir, "lfr");
    SEN_line_install (&ld, lfr, "line");
    CLI_exec_init (lfr, &mstart, "mstart", "DC Motor start", mstart_exec);
    CLI_exec_init (lfr, &mstop, "mstop", "DC Motor stop", mstop_exec);
    CLI_exec_init (lfr, &mgo, "mgo", "DC Motor go +|-U", mgo_exec);
    CLI_exec_init (lfr, &mturn, "mturn", "DC Motor turn +|-U", mturn_exec);
}
