/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>

#include <arch/pins.h>
#include <arch/digio.h>

DEV_digout_t SAM_run_led;
DEV_digout_t SAM_green_led;
DEV_digout_t SAM_red_led;
DEV_digout_t SAM_lcd_light;
DEV_digout_t SAM_lcd_reset;
DEV_digout_t SAM_phy_power_down;
DEV_digout_t SAM_pin_B29;

static void
set_run_led (void)
{
    AT91C_BASE_PIOB->PIO_SODR = B_RUN_LED;
}

static void
clear_run_led (void)
{
    AT91C_BASE_PIOB->PIO_CODR = B_RUN_LED;
}

static void
set_green_led (void)
{
    AT91C_BASE_PIOA->PIO_SODR = A_GREEN_LED;
}

static void
clear_green_led (void)
{
    AT91C_BASE_PIOA->PIO_CODR = A_GREEN_LED;
}

static void
set_red_led (void)
{
    AT91C_BASE_PIOA->PIO_SODR = A_RED_LED;
}

static void
clear_red_led (void)
{
    AT91C_BASE_PIOA->PIO_CODR = A_RED_LED;
}

static void
set_lcd_light (void)
{
    AT91C_BASE_PIOB->PIO_SODR = B_LCD_BL;
}

static void
clear_lcd_light (void)
{
    AT91C_BASE_PIOB->PIO_CODR = B_LCD_BL;
}

static void
set_lcd_reset (void)
{
    AT91C_BASE_PIOA->PIO_SODR = A_LCD_RST;
}

static void
clear_lcd_reset (void)
{
    AT91C_BASE_PIOA->PIO_CODR = A_LCD_RST;
}

static void
set_phy_power_down (void)
{
    AT91C_BASE_PIOB->PIO_SODR = B_PHY_POWER_DOWN;
}

static void
clear_phy_power_down (void)
{
    AT91C_BASE_PIOB->PIO_CODR = B_PHY_POWER_DOWN;
}

static void
set_pin_B29 (void)
{
    AT91C_BASE_PIOB->PIO_SODR = B_PIN_B29;
}

static void
clear_pin_B29 (void)
{
    AT91C_BASE_PIOB->PIO_CODR = B_PIN_B29;
}


DEV_digin_t SAM_switch_boot;
DEV_digin_t SAM_switch_1;
DEV_digin_t SAM_switch_2;
DEV_digin_t SAM_card_write;
DEV_digin_t SAM_card_insert;
DEV_digin_t SAM_joystick_up;
DEV_digin_t SAM_joystick_down;
DEV_digin_t SAM_joystick_left;
DEV_digin_t SAM_joystick_right;
DEV_digin_t SAM_joystick_center;
DEV_digin_t SAM_pin_B27;

static unsigned long
sample_switch_boot (void)
{
    return *AT91C_PIOB_PDSR & B_SW_BOOT;
}

static unsigned long
sample_switch_1 (void)
{
    return *AT91C_PIOB_PDSR & B_SW1;
}

static unsigned long
sample_switch_2 (void)
{
    return *AT91C_PIOB_PDSR & B_SW2;
}

static unsigned long
sample_card_write (void)
{
    return *AT91C_PIOB_PDSR & B_WP;
}

static unsigned long
sample_card_insert (void)
{
    return *AT91C_PIOB_PDSR & B_CP;
}

static unsigned long
sample_joystick_up (void)
{
    return *AT91C_PIOA_PDSR & A_JS_UP;
}

static unsigned long
sample_joystick_down (void)
{
    return *AT91C_PIOA_PDSR & A_JS_DOWN;
}

static unsigned long
sample_joystick_left (void)
{
    return *AT91C_PIOA_PDSR & A_JS_LEFT;
}

static unsigned long
sample_joystick_right (void)
{
    return *AT91C_PIOA_PDSR & A_JS_RIGHT;
}

static unsigned long
sample_joystick_center (void)
{
    return *AT91C_PIOA_PDSR & A_JS_CENTER;
}

static unsigned long
sample_pin_B27 (void)
{
    return *AT91C_PIOB_PDSR & B_PIN_B27;
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
    DEV_digout_init (&SAM_pin_B29, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_pin_B29, clear_pin_B29);

    DEV_diginputs_init (&SAM_control_in);
    DEV_digin_init (NULL, &SAM_switch_boot, DEV_DIGIO_NEG, sample_switch_boot, 0);
    DEV_digin_init (&SAM_control_in, &SAM_switch_1, DEV_DIGIO_NEG, sample_switch_1, 1);
    DEV_digin_init (&SAM_control_in, &SAM_switch_2, DEV_DIGIO_NEG, sample_switch_2, 1);
    DEV_digin_init (NULL, &SAM_card_write, DEV_DIGIO_NEG, sample_card_write, 0);
    DEV_digin_init (&SAM_control_in, &SAM_card_insert, DEV_DIGIO_NEG, sample_card_insert, 1);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_up, DEV_DIGIO_NEG, sample_joystick_up, 1);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_down, DEV_DIGIO_NEG, sample_joystick_down, 1);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_left, DEV_DIGIO_NEG, sample_joystick_left, 1);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_right, DEV_DIGIO_NEG, sample_joystick_right, 1);
    DEV_digin_init (&SAM_control_in, &SAM_joystick_center, DEV_DIGIO_NEG, sample_joystick_center,1);
    DEV_digin_init (NULL, &SAM_pin_B27, DEV_DIGIO_POS, sample_pin_B27, 0);
}

extern void
SAM_digio_install (void)
{
    DEV_digout_install (&SAM_run_led, "run");
    DEV_digout_install (&SAM_green_led, "green");
    DEV_digout_install (&SAM_red_led, "red");
    DEV_digout_install (&SAM_lcd_light, "lcd_lit");
    DEV_digout_install (&SAM_lcd_reset, "lcd_rst");
    DEV_digout_install (&SAM_phy_power_down, "phy_pd");
    DEV_digout_install (&SAM_pin_B29, "pin B29");

    DEV_digin_install (&SAM_switch_boot, "boot");
    DEV_digin_install (&SAM_switch_1, "button1");
    DEV_digin_install (&SAM_switch_2, "button2");
    DEV_digin_install (&SAM_card_write, "card_w");
    DEV_digin_install (&SAM_card_insert, "card_in");
    DEV_digin_install (&SAM_joystick_up, "j_up");
    DEV_digin_install (&SAM_joystick_down, "j_down");
    DEV_digin_install (&SAM_joystick_left, "j_left");
    DEV_digin_install (&SAM_joystick_right, "j_right");
    DEV_digin_install (&SAM_joystick_center, "j_press");
    DEV_digin_install (&SAM_pin_B27, "pin B27");
}
