/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <dev/arch/at91sam7a2/csp.h>

#include <arch/pins.h>
#include <arch/digio.h>


DEV_digout_t LA2_run_led;
DEV_digout_t LA2_green_led;
DEV_digout_t LA2_red_led;
DEV_digout_t LA2_lan_reset;
DEV_digout_t LA2_m1_a;
DEV_digout_t LA2_m1_b;
DEV_digout_t LA2_m2_a;
DEV_digout_t LA2_m2_b;
DEV_digout_t LA2_led_bar1_clk;
DEV_digout_t LA2_led_bar1_data;
DEV_digout_t LA2_led_bar2_clk;
DEV_digout_t LA2_led_bar2_data;
DEV_digout_t LA2_ultra_sonic;
DEV_digout_t LA2_output_1;
DEV_digout_t LA2_output_2;


static void
set_run_led (void)
{
    UPIO->SODR   = RUN_LED;
}

static void
clear_run_led (void)
{
    UPIO->CODR   = RUN_LED;
}

static void
set_green_led (void)
{
    UPIO->SODR   = GREEN_LED;
}

static void
clear_green_led (void)
{
    UPIO->CODR   = GREEN_LED;
}

static void
set_red_led (void)
{
    UPIO->SODR   = RED_LED;
}

static void
clear_red_led (void)
{
    UPIO->CODR   = RED_LED;
}


static void
set_lan_reset (void)
{
    UPIO->SODR   = LAN_RESET;
}

static void
clear_lan_reset (void)
{
    UPIO->CODR   = LAN_RESET;
}

static void
set_m1_a (void)
{
    UPIO->SODR   = M1_A;
}

static void
clear_m1_a (void)
{
    UPIO->CODR   = M1_A;
}

static void
set_m1_b (void)
{
    UPIO->SODR   = M1_B;
}

static void
clear_m1_b (void)
{
    UPIO->CODR   = M1_B;
}

static void
set_m2_a (void)
{
    UPIO->SODR   = M2_A;
}

static void
clear_m2_a (void)
{
    UPIO->CODR   = M2_A;
}

static void
set_m2_b (void)
{
    UPIO->SODR   = M2_B;
}

static void
clear_m2_b (void)
{
    UPIO->CODR   = M2_B;
}

static void
set_led_bar1_clk (void)
{
    UPIO->SODR   = LED_BAR1_CLK;
}

static void
clear_led_bar1_clk (void)
{
    UPIO->CODR   = LED_BAR1_CLK;
}

static void
set_led_bar1_data (void)
{
    UPIO->SODR   = LED_BAR1_DATA;
}

static void
clear_led_bar1_data (void)
{
    UPIO->CODR   = LED_BAR1_DATA;
}

static void
set_led_bar2_clk (void)
{
    UPIO->SODR   = LED_BAR2_CLK;
}

static void
clear_led_bar2_clk (void)
{
    UPIO->CODR   = LED_BAR2_CLK;
}

static void
set_led_bar2_data (void)
{
    UPIO->SODR   = LED_BAR2_DATA;
}

static void
clear_led_bar2_data (void)
{
    UPIO->CODR   = LED_BAR2_DATA;
}

static void
set_ultra_sonic (void)
{
    UPIO->SODR   = ULTRA_SONIC;
}

static void
clear_ultra_sonic (void)
{
    UPIO->CODR   = ULTRA_SONIC;
}

static void
set_output_1 (void)
{
    UPIO->SODR   = OUTPUT_1;
}

static void
clear_output_1 (void)
{
    UPIO->CODR   = OUTPUT_1;
}

static void
set_output_2 (void)
{
    UPIO->SODR   = OUTPUT_2;
}

static void
clear_output_2 (void)
{
    UPIO->CODR   = OUTPUT_2;
}




DEV_digin_t LA2_card_write;
DEV_digin_t LA2_card_insert;
DEV_digin_t LA2_switch;
DEV_digin_t LA2_pushbutton;
DEV_digin_t LA2_line_1;
DEV_digin_t LA2_line_2;
DEV_digin_t LA2_side_line_1;
DEV_digin_t LA2_side_line_2;
DEV_digin_t LA2_input_1;
DEV_digin_t LA2_input_2;


static unsigned long
sample_card_write (void)
{
	return CSP_PIOGetStatus(UPIO) & MMC_WP;
}

static unsigned long
sample_card_insert (void)
{
	return CSP_PIOGetStatus(UPIO) & MMC_CP;
}

static unsigned long
sample_switch (void)
{
    return CSP_PIOGetStatus(UPIO) & SWITCH;
}

static unsigned long
sample_pushbutton (void)
{
    return CSP_PIOGetStatus(UPIO) & PUSHBUTTON;
}

static unsigned long
sample_line_1 (void)
{
    return CSP_PIOGetStatus(UPIO) & LINE_1;
}

static unsigned long
sample_line_2 (void)
{
    return CSP_PIOGetStatus(UPIO) & LINE_2;
}

static unsigned long
sample_side_line_1 (void)
{
    return CSP_PIOGetStatus(UPIO) & SIDE_LINE_1;
}

static unsigned long
sample_side_line_2 (void)
{
    return CSP_PIOGetStatus(UPIO) & SIDE_LINE_2;
}

static unsigned long
sample_input_1 (void)
{
    return CSP_PIOGetStatus(UPIO) & INPUT_1;
}

static unsigned long
sample_input_2 (void)
{
    return CSP_PIOGetStatus(UPIO) & INPUT_2;
}



DEV_diginputs_t LA2_control_in;



extern void
LA2_digio_init (void)
{
    DEV_digout_init (&LA2_run_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_run_led, clear_run_led);
    DEV_digout_init (&LA2_green_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_green_led, clear_green_led);
    DEV_digout_init (&LA2_red_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_red_led, clear_red_led);
    DEV_digout_init (&LA2_lan_reset, DEV_DIGIO_HIGH, DEV_DIGIO_NEG, set_lan_reset, clear_lan_reset);
    DEV_digout_init (&LA2_m1_a, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_m1_a, clear_m1_a);
    DEV_digout_init (&LA2_m1_b, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_m1_b, clear_m1_b);
    DEV_digout_init (&LA2_m2_a, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_m2_a, clear_m2_a);
    DEV_digout_init (&LA2_m2_b, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_m2_b, clear_m2_b);
    DEV_digout_init (&LA2_led_bar1_clk, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_led_bar1_clk, clear_led_bar1_clk);
    DEV_digout_init (&LA2_led_bar1_data, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_led_bar1_data, clear_led_bar1_data);
    DEV_digout_init (&LA2_led_bar2_clk, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_led_bar2_clk, clear_led_bar2_clk);
    DEV_digout_init (&LA2_led_bar2_data, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_led_bar2_data, clear_led_bar2_data);
    DEV_digout_init (&LA2_ultra_sonic, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_ultra_sonic, clear_ultra_sonic);
    DEV_digout_init (&LA2_output_1, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_output_1, clear_output_1);
    DEV_digout_init (&LA2_output_2, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_output_2, clear_output_2);

    DEV_diginputs_init (&LA2_control_in);
    DEV_digin_init (&LA2_control_in, &LA2_card_write, DEV_DIGIO_POS, sample_card_write, 0);
    DEV_digin_init (&LA2_control_in, &LA2_card_insert, DEV_DIGIO_POS, sample_card_insert, 1);
    DEV_digin_init (&LA2_control_in, &LA2_switch, DEV_DIGIO_NEG, sample_switch, 0);
    DEV_digin_init (&LA2_control_in, &LA2_pushbutton, DEV_DIGIO_NEG, sample_pushbutton, 3);
    DEV_digin_init (&LA2_control_in, &LA2_line_1, DEV_DIGIO_NEG, sample_line_1, 0);
    DEV_digin_init (&LA2_control_in, &LA2_line_2, DEV_DIGIO_NEG, sample_line_2, 0);
    DEV_digin_init (&LA2_control_in, &LA2_side_line_1, DEV_DIGIO_NEG, sample_side_line_1, 0);
    DEV_digin_init (&LA2_control_in, &LA2_side_line_2, DEV_DIGIO_NEG, sample_side_line_2, 0);
    DEV_digin_init (&LA2_control_in, &LA2_input_1, DEV_DIGIO_NEG, sample_input_1, 2);
    DEV_digin_init (&LA2_control_in, &LA2_input_2, DEV_DIGIO_NEG, sample_input_2, 2);
}

extern void
LA2_digio_install (void)
{
    DEV_digout_install (&LA2_run_led, "run");
    DEV_digout_install (&LA2_green_led, "green");
    DEV_digout_install (&LA2_red_led, "red");
    DEV_digout_install (&LA2_lan_reset, "lan_rst");
    DEV_digout_install (&LA2_m1_a, "m1_a");
    DEV_digout_install (&LA2_m1_b, "m1_b");
    DEV_digout_install (&LA2_m2_a, "m2_a");
    DEV_digout_install (&LA2_m2_b, "m2_b");
    DEV_digout_install (&LA2_ultra_sonic, "u_sonic");
    DEV_digout_install (&LA2_output_1, "out_1");
    DEV_digout_install (&LA2_output_2, "out_2");

    DEV_digin_install (&LA2_card_write, "card_w");
    DEV_digin_install (&LA2_card_insert, "card_in");
    DEV_digin_install (&LA2_switch, "switch");
    DEV_digin_install (&LA2_pushbutton, "pushbut");
    DEV_digin_install (&LA2_line_1, "line_1");
    DEV_digin_install (&LA2_line_2, "line_2");
    DEV_digin_install (&LA2_side_line_1, "sline_1");
    DEV_digin_install (&LA2_side_line_2, "sline_2");
    DEV_digin_install (&LA2_input_1, "in_1");
    DEV_digin_install (&LA2_input_2, "in_2");
}
