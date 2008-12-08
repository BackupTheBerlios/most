/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "arch/pins.h"
#include "arch/digio.h"

DEV_digout_t SAM_run_led;
DEV_digout_t SAM_green_led;
DEV_digout_t SAM_red_led;
DEV_digout_t SAM_lcd_light;
DEV_digout_t SAM_lcd_reset;
DEV_digout_t SAM_phy_power_down;

static void
set_run_led (void)
{
	AT91C_BASE_PIOB->PIO_SODR = AT91B_RUN_LED;
}

static void
clear_run_led (void)
{
    AT91C_BASE_PIOB->PIO_CODR = AT91B_RUN_LED;
}

static void
set_green_led (void)
{
	AT91C_BASE_PIOA->PIO_SODR = AT91A_GREEN_LED;
}

static void
clear_green_led (void)
{
    AT91C_BASE_PIOA->PIO_CODR = AT91A_GREEN_LED;
}

static void
set_red_led (void)
{
	AT91C_BASE_PIOA->PIO_SODR = AT91A_RED_LED;
}

static void
clear_red_led (void)
{
    AT91C_BASE_PIOA->PIO_CODR = AT91A_RED_LED;
}

static void
set_lcd_light (void)
{
	AT91C_BASE_PIOB->PIO_SODR = AT91B_LCD_BL;
}

static void
clear_lcd_light (void)
{
    AT91C_BASE_PIOB->PIO_CODR = AT91B_LCD_BL;
}

static void
set_lcd_reset (void)
{
	AT91C_BASE_PIOB->PIO_SODR = AT91B_LCD_RST;
}

static void
clear_lcd_reset (void)
{
    AT91C_BASE_PIOB->PIO_CODR = AT91B_LCD_RST;
}

static void
set_phy_power_down (void)
{
	AT91C_BASE_PIOB->PIO_SODR = AT91B_PHY_POWER_DOWN;
}

static void
clear_phy_power_down (void)
{
    AT91C_BASE_PIOB->PIO_CODR = AT91B_PHY_POWER_DOWN;
}

DEV_digin_t SAM_switch_1;
DEV_digin_t SAM_switch_2;
DEV_digin_t SAM_joystick_up;
DEV_digin_t SAM_joystick_down;
DEV_digin_t SAM_joystick_left;
DEV_digin_t SAM_joystick_right;
DEV_digin_t SAM_joystick_center;

static unsigned long
sample_switch_1 (void)
{
    //return AT91C_PIOB_PDSR & AT91B_SW1;
    return 0;
}

static unsigned long
sample_switch_2 (void)
{
    //return AT91C_PIOB_PDSR & AT91B_SW2;
    return 0;
}

static unsigned long
sample_joystick_up (void)
{
    //return AT91C_PIOA_PDSR & AT91A_JS_UP;
    return 0;
}

static unsigned long
sample_joystick_down (void)
{
    //return AT91C_PIOA_PDSR & AT91A_JS_DOWN;
    return 0;
}

static unsigned long
sample_joystick_left (void)
{
    //return AT91C_PIOA_PDSR & AT91A_JS_LEFT;
    return 0;
}

static unsigned long
sample_joystick_right (void)
{
    //return AT91C_PIOA_PDSR & AT91A_JS_RIGHT;
    return 0;
}

static unsigned long
sample_joystick_center (void)
{
    //return AT91C_PIOA_PDSR & AT91A_JS_CENTER;
    return 0;
}

DEV_diginputs_t SAM_control_in;

extern void
SAM_digio_init (void)
{
    DEV_digout_init (&SAM_run_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_run_led, clear_run_led);
    DEV_digout_init (&SAM_green_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_green_led, clear_green_led);
    DEV_digout_init (&SAM_red_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_red_led, clear_red_led);
    DEV_digout_init (&SAM_lcd_light, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_lcd_light, clear_lcd_light);
	DEV_digout_init (&SAM_lcd_reset, DEV_DIGIO_HIGH, DEV_DIGIO_POS, set_lcd_reset, clear_lcd_reset);
    DEV_digout_init (&SAM_phy_power_down, DEV_DIGIO_HIGH, DEV_DIGIO_NEG, set_phy_power_down, clear_phy_power_down);
	DEV_digout_init (&SAM_lcd_reset, DEV_DIGIO_HIGH, DEV_DIGIO_POS, set_lcd_reset, clear_lcd_reset);

	DEV_diginputs_init (&SAM_control_in);
    DEV_digin_init (&SAM_control_in, &SAM_switch_1, DEV_DIGIO_NEG, sample_switch_1, 0);
    DEV_digin_init (&SAM_control_in, &SAM_switch_2, DEV_DIGIO_NEG, sample_switch_2, 0);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_up, DEV_DIGIO_NEG, sample_joystick_up, 0);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_down, DEV_DIGIO_NEG, sample_joystick_down, 0);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_left, DEV_DIGIO_NEG, sample_joystick_left, 0);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_right, DEV_DIGIO_NEG, sample_joystick_right, 0);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_center, DEV_DIGIO_NEG, sample_joystick_center, 0);
}
