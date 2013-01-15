/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <uso/scheduler.h>
#include <dev/digin.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/arch/h8/H83003_REG.h>
#include <dev/arch/h8/H83003.h>
#include <dev/arch/h8/bitops.h>

#include "arch/ticks.h"
#include "arch/cpu.h"

unsigned long loop_count;

extern void
MDC_ticks_init (void)
{
    USO_ticks_per_sec = MDC_TICKS_IN_HZ;
    // Set in timer mode
    H8_RFSHCR &= ~(H8_RFSHCR_PSRAME | H8_RFSHCR_DRAME);
    // Set the compare count for 1 msec timer
    H8_RTCOR = 125;             // 16000 (16MHz) / 128 = 125
    /** Set WDT and refresh controller requests to high priority */
    H8_IPRA |=  H8_IPRA_IPRA3;
    // Set the clock source  to CPU clock and the
    // divider to 128 ,enable the interrupt (CMIE) and
    // clear Compare Match Flag (CMF)
    H8_RTMCSR = H8_RTMCSR_CMIE | H8_RTMCSR_CKS2;
    DEV_clock_init (NULL);
}

extern void ACE_INTERRUPT_
MDC_CMI_ISR (void) ACE_SECTION_ (".unref");

extern void ACE_INTERRUPT_
MDC_CMI_ISR (void)
{
    // Clear the timer compare flag.
    H8_BITCLEAR (H8_RTMCSR_CMF, H8_RTMCSR);

    /* Take care!
     * USO_preemt, USO_wakeup, DEV_timer_fire may have a context switch as result.
     * This normal happens (the tick_interrupt occurs while cpu is in idle thread and
     * other threads have to be scheduled) and it switches to the context of the other thread
     * maybe enabling the interrupts and this routine is reentered at the next tick interrupt.
     */
    DEV_clock_tick ();
    DEV_timer_fire ();
    USO_wakeup ();
    USO_preempt ();
}
