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



DEV_digin_t LA2_card_write;
DEV_digin_t LA2_card_insert;

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


DEV_diginputs_t LA2_control_in;

extern void
LA2_digio_init (void)
{
    DEV_digout_init (&LA2_run_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_run_led, clear_run_led);
    DEV_digout_init (&LA2_green_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_green_led, clear_green_led);
    DEV_digout_init (&LA2_red_led, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_red_led, clear_red_led);
    DEV_digout_init (&LA2_lan_reset, DEV_DIGIO_HIGH, DEV_DIGIO_NEG, set_lan_reset, clear_lan_reset);

    DEV_diginputs_init (&LA2_control_in);
    DEV_digin_init (&LA2_control_in, &LA2_card_write, DEV_DIGIO_POS, sample_card_write, 0);
    DEV_digin_init (&LA2_control_in, &LA2_card_insert, DEV_DIGIO_POS, sample_card_insert, 1);
}

extern void
LA2_digio_install (void)
{
    DEV_digout_install (&LA2_run_led, "run");
    DEV_digout_install (&LA2_green_led, "green");
    DEV_digout_install (&LA2_red_led, "red");
    DEV_digout_install (&LA2_lan_reset, "lan_rst");

    DEV_digin_install (&LA2_card_write, "card_w");
    DEV_digin_install (&LA2_card_insert, "card_in");
}
