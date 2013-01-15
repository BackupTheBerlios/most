/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <dev/serial.h>
#include <dev/serial_settings.h>
#include <dev/arch/h8/sci.h>

#include "arch/sci.h"

static DEV_serial_t ser0;
static struct DEV_serial_settings ser0_settings;

static DEV_serial_t ser1;
static struct DEV_serial_settings ser1_settings;

static void
open_0 (const struct DEV_serial_settings *settings)
{
    DEV_h8_sci_open (0, settings);
}

static void
open_1 (const struct DEV_serial_settings *settings)
{
    DEV_h8_sci_open (1, settings);
}

static void
close_0 (void)
{
    DEV_h8_sci_close (0);
}

static void
close_1 (void)
{
    DEV_h8_sci_close (1);
}

static void
start_0 (void)
{
    DEV_h8_sci_start (0, &ser0);
}

static void
start_1 (void)
{
    DEV_h8_sci_start (1, &ser1);
}


void
MDC_sci_init_0 (void)
{
    DEV_serial_settings_init (&ser0_settings);
    ser0_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser0, &ser0_settings, open_0, close_0, start_0, TRUE, "ser0");
}

void
MDC_sci_init_1 (void)
{
    DEV_serial_settings_init (&ser1_settings);
    ser1_settings.baud = DEV_SER_BAUD_9600;
    DEV_serial_init (&ser1, &ser1_settings, open_1, close_1, start_1, TRUE, "ser1");
}


extern void ACE_INTERRUPT_
MDC_TXE0_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_TXE0_ISR (void)
{
    DEV_h8_sci_txe (0, &ser0);
}

extern void ACE_INTERRUPT_
MDC_TXI0_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_TXI0_ISR (void)
{
    DEV_h8_sci_txi (0, &ser0);
}

extern void ACE_INTERRUPT_
MDC_RXI0_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_RXI0_ISR (void)
{
    DEV_h8_sci_rxi (0, &ser0);
}

extern void ACE_INTERRUPT_
MDC_ERI0_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_ERI0_ISR (void)
{
    DEV_h8_sci_eri (0, &ser0);
}


extern void ACE_INTERRUPT_
MDC_TXE1_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_TXE1_ISR (void)
{
    DEV_h8_sci_txe (1, &ser1);
}

extern void ACE_INTERRUPT_
MDC_TXI1_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_TXI1_ISR (void)
{
    DEV_h8_sci_txi (1, &ser1);
}

extern void ACE_INTERRUPT_
MDC_RXI1_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_RXI1_ISR (void)
{
    DEV_h8_sci_rxi (1, &ser1);
}

extern void ACE_INTERRUPT_
MDC_ERI1_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_ERI1_ISR (void)
{
    DEV_h8_sci_eri (1, &ser1);
}
