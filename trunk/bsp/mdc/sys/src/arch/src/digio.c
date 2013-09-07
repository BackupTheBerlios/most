/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <dev/arch/h83003/H83003_REG.h>

#include <arch/pins.h>
#include <arch/digio.h>


DEV_digout_t MDC_run_led;
DEV_digout_t MDC_eth_reset;
DEV_digout_t MDC_red_led;
DEV_digout_t MDC_green_led;
DEV_digout_t MDC_lcd_backlight;

DEV_digin_t MDC_button;
DEV_digin_t MDC_jumper_1;
DEV_digin_t MDC_jumper_2;
DEV_digin_t MDC_jumper_3;
DEV_digin_t MDC_jumper_4;
DEV_digin_t MDC_random_signal;
DEV_digin_t MDC_switch;


static void
set_run_led (void)
{
    H8_PBDR |= H8_PB7_RUN_LED;
}

static void
clear_run_led (void)
{
    H8_PBDR &= ~H8_PB7_RUN_LED;
}

static void
set_eth_reset (void)
{
    H8_PADR |= H8_PA1_RESETETH;
}

static void
clear_eth_reset (void)
{
    H8_PADR &= ~H8_PA1_RESETETH;
}

static void
set_red_led (void)
{
    H8_P4DR |= H8_P43_RED_LED;
}

static void
clear_red_led (void)
{
    H8_P4DR &= ~H8_P43_RED_LED;
}

static void
set_green_led (void)
{
    H8_P4DR |= H8_P42_GREEN_LED;
}

static void
clear_green_led (void)
{
    H8_P4DR &= ~H8_P42_GREEN_LED;
}

static void
set_lcd_backlight (void)
{
    H8_PADR |= H8_PA4_PWM1;
}

static void
clear_lcd_backlight (void)
{
    H8_PADR &= ~H8_PA4_PWM1;
}



static unsigned long
sample_button (void)
{
    return H8_P7DR & H8_P76_BUTTON;
}

static unsigned long
sample_jumper_1 (void)
{
    return H8_P7DR & H8_P70_JUMPER_1;
}

static unsigned long
sample_jumper_2 (void)
{
    return H8_P7DR & H8_P71_JUMPER_2;
}

static unsigned long
sample_jumper_3 (void)
{
    return H8_P7DR & H8_P72_JUMPER_3;
}

static unsigned long
sample_jumper_4 (void)
{
    return H8_P7DR & H8_P73_JUMPER_4;
}

static unsigned long
sample_random_signal (void)
{
    return H8_P8DR & H8_P83_CS1_IRQ3;
}

static unsigned long
sample_switch (void)
{
    return H8_P4DR & H8_P44_SWITCH;
}




DEV_diginputs_t MDC_control_in;

extern void
MDC_digio_init (void)
{
    DEV_digout_init (&MDC_run_led, DEV_DIGIO_HIGH, DEV_DIGIO_NEG, set_run_led, clear_run_led);
    DEV_digout_init (&MDC_eth_reset, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_eth_reset, clear_eth_reset);
    DEV_digout_init (&MDC_red_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_red_led, clear_red_led);
    DEV_digout_init (&MDC_green_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_green_led, clear_green_led);
    DEV_digout_init (&MDC_lcd_backlight, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_lcd_backlight, clear_lcd_backlight);

    DEV_diginputs_init (&MDC_control_in);
    DEV_digin_init (&MDC_control_in, &MDC_button, DEV_DIGIO_NEG, sample_button, 0);
    DEV_digin_init (&MDC_control_in, &MDC_jumper_1, DEV_DIGIO_NEG, sample_jumper_1, 0);
    DEV_digin_init (&MDC_control_in, &MDC_jumper_2, DEV_DIGIO_NEG, sample_jumper_2, 0);
    DEV_digin_init (&MDC_control_in, &MDC_jumper_3, DEV_DIGIO_NEG, sample_jumper_3, 0);
    DEV_digin_init (&MDC_control_in, &MDC_jumper_4, DEV_DIGIO_NEG, sample_jumper_4, 0);
    DEV_digin_init (&MDC_control_in, &MDC_random_signal, DEV_DIGIO_POS, sample_random_signal, 0);
    DEV_digin_init (&MDC_control_in, &MDC_switch, DEV_DIGIO_NEG, sample_switch, 0);
}

extern void
MDC_digio_install (void)
{
    DEV_digout_install (&MDC_run_led, "run");
    DEV_digout_install (&MDC_eth_reset, "eth_rst");
    DEV_digout_install (&MDC_red_led, "red");
    DEV_digout_install (&MDC_green_led, "green");
    DEV_digout_install (&MDC_lcd_backlight, "lcd_bl");

    DEV_digin_install (&MDC_button, "button");
    DEV_digin_install (&MDC_jumper_1, "jump1");
    DEV_digin_install (&MDC_jumper_2, "jump2");
    DEV_digin_install (&MDC_jumper_3, "jump3");
    DEV_digin_install (&MDC_jumper_4, "jump4");
    DEV_digin_install (&MDC_random_signal, "rand");
    DEV_digin_install (&MDC_switch, "switch");
}
