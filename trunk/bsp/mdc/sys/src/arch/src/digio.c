/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <dev/arch/h8300/io_ports.h>

#include "arch/pins.h"
#include "arch/digio.h"

DEV_digout_t MDC_run_led;
DEV_digout_t MDC_eth_reset;
DEV_digout_t MDC_ctrl_led_1;
DEV_digout_t MDC_ctrl_led_2;
DEV_digout_t MDC_ctrl_led_3;
DEV_digout_t MDC_ctrl_led_4;
DEV_digout_t MDC_ctrl_led_5;

static void
set_run_led (void)
{
    h8_PBDR |= H8_PB7_CPU_LED;
}

static void
clear_run_led (void)
{
    h8_PBDR &= ~H8_PB7_CPU_LED;
}

static void
set_eth_reset (void)
{
    h8_PADR |= H8_PA1_RESETETH;
}

static void
clear_eth_reset (void)
{
    h8_PADR &= ~H8_PA1_RESETETH;
}

static void
set_1 (void)
{
    h8_P4DR |= H8_P43_JTAG_TCK;
}

static void
clear_1 (void)
{
    h8_P4DR &= ~H8_P43_JTAG_TCK;
}

static void
set_2 (void)
{
    h8_P4DR |= H8_P42_JTAG_TMS;
}

static void
clear_2 (void)
{
    h8_P4DR &= ~H8_P42_JTAG_TMS;
}

static void
set_3 (void)
{
    h8_PBDR |= H8_PB2_PWM4;
}

static void
clear_3 (void)
{
    h8_PBDR &= ~H8_PB2_PWM4;
}

static void
set_4 (void)
{
    h8_PBDR |= H8_PB0_PWM3;
}

static void
clear_4 (void)
{
    h8_PBDR &= ~H8_PB0_PWM3;
}

static void
set_5 (void)
{
    h8_PADR |= H8_PA4_PWM1;
}

static void
clear_5 (void)
{
    h8_PADR &= ~H8_PA4_PWM1;
}

DEV_digin_t MDC_button;
DEV_digin_t MDC_jumper_1;
DEV_digin_t MDC_jumper_2;
DEV_digin_t MDC_jumper_3;
DEV_digin_t MDC_jumper_4;

static unsigned long
sample_button (void)
{
    return h8_P7DR & H8_P76_JAPO;
}

static unsigned long
sample_1 (void)
{
    return h8_P7DR & H8_P70_DR1;
}

static unsigned long
sample_2 (void)
{
    return h8_P7DR & H8_P71_DR2;
}

static unsigned long
sample_3 (void)
{
    return h8_P7DR & H8_P72_DR3;
}

static unsigned long
sample_4 (void)
{
    return h8_P7DR & H8_P73_CNDRS;
}


extern void
MDC_digio_init (void)
{
    DEV_digout_init (&MDC_run_led, DEV_DIGIO_HIGH, DEV_DIGIO_NEG, set_run_led,
                     clear_run_led);
    DEV_digout_init (&MDC_eth_reset, DEV_DIGIO_LOW, DEV_DIGIO_POS,
                     set_eth_reset, clear_eth_reset);
    DEV_digout_init (&MDC_ctrl_led_1, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_1, clear_1);
    DEV_digout_init (&MDC_ctrl_led_2, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_2, clear_2);
    DEV_digout_init (&MDC_ctrl_led_3, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_3, clear_3);
    DEV_digout_init (&MDC_ctrl_led_4, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_4, clear_4);
    DEV_digout_init (&MDC_ctrl_led_5, DEV_DIGIO_LOW, DEV_DIGIO_NEG, set_5, clear_5);

    DEV_digin_init (&MDC_button, DEV_DIGIO_NEG, sample_button, 10);
    DEV_digin_init (&MDC_jumper_1, DEV_DIGIO_NEG, sample_1, 10);
    DEV_digin_init (&MDC_jumper_2, DEV_DIGIO_NEG, sample_2, 10);
    DEV_digin_init (&MDC_jumper_3, DEV_DIGIO_NEG, sample_3, 10);
    DEV_digin_init (&MDC_jumper_4, DEV_DIGIO_NEG, sample_4, 10);
}
