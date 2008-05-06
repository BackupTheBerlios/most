/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <uso/scheduler.h>
#include <uso/sleep.h>
#include <dev/digin.h>
#include <dev/timer.h>
#include <dev/clock.h>

#include "arch/OLIMEX_SAM7_EX256.h"
#include "init/init.h"
#include "arch/ticks.h"

unsigned long loop_count;

extern void
SAM_ticks_init (void)
{
    AT91C_BASE_PITC->PITC_PIMR = (48/16) * 1000;
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN | AT91C_PITC_PITIEN;
}


extern void SAM_ticks_interrupt (void)
{
	unsigned int pivr;
    USO_preempt ();
    USO_wakeup ();
    DEV_timer_fire ();
	DEV_clock_tick ();
	pivr = AT91C_BASE_PITC->PITC_PIVR;
}
