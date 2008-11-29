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
#include <dev/arch/h8300/io_ports.h>
#include <dev/arch/h8300/io_def.h>

#include "arch/ticks.h"
#include "arch/cpu.h"

unsigned long loop_count;

extern void
MDC_ticks_init (void)
{
    // Set in timer mode
    h8_RFSHCR &= ~(H8_RFSHCR_PSRAME | H8_RFSHCR_DRAME);
    // Set the compare count for 1 msec timer
    h8_RTCOR = 125; // 16000 (16MHz) / 128 = 125
    // Set the refresh timer interrupt priority to high
    MDC_SET_PRIORITY_WDT_RFSH ();
    // Set the clock source  to CPU clock and the
    // divider to 128 ,enable the interrupt (CMIE) and
    // clear Compare Match Flag (CMF)
    h8_RTMCSR = H8_RTMCSR_CMIE | H8_RTMCSR_CKS2;
}


extern void _INTERRUPT_ MDC_CMI_ISR (void)
{
    USO_preempt ();
    USO_wakeup ();
    DEV_timer_fire ();
	DEV_clock_tick ();
    // Clear the timer compare flag.
    H8_BITCLEAR (H8_RTMCSR_CMF, h8_RTMCSR);
}
