/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/assert.h>
#include <uso/scheduler.h>
#include <dev/digin.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/arch/at91/AT91SAM7X256.h>
#include <dev/arch/at91/pit.h>

#include "init/init.h"
#include "arch/ticks.h"
#include "arch/cpu.h"

static unsigned int piv = 0;
static unsigned int piv_av = 0;

static unsigned int picnt = 0;
static unsigned int pit_overrun = 0;
static unsigned int ovr_cnt = 0;


static unsigned long
get_us (void)
{
    unsigned int piv;
    unsigned int picnt;
    unsigned long us;
    piv = DEV_at91_PIT_get_PIIR (&picnt);
    us = DEV_at91_PIT_ticks_2_usec (piv);
    us += picnt * (DEV_AT91_PIT_SEC_IN_USEC / SAM_TICKS_IN_HZ);
    return us;
}

extern void
SAM_ticks_init (void)
{
    USO_ticks_per_sec = SAM_TICKS_IN_HZ;
    DEV_at91_PIT_init (SAM_TICKS_IN_HZ, SAM_MCK_IN_MHZ);
    DEV_at91_PIT_enable_IT ();
    DEV_clock_init (get_us);
}

extern void
SAM_ticks_debug (void)
{
    ACE_printf ("Ticks interrupt time:\n"
                "\tPIV cur: %d us, av: %d us\n"
                "\tPICNT: cur: %d, ovr: %d, ovr_cnt: %d\n",
                DEV_at91_PIT_ticks_2_usec (piv),
                DEV_at91_PIT_ticks_2_usec (piv_av), picnt, pit_overrun, ovr_cnt);
}

extern void
SAM_ticks_interrupt (void)
{
    static unsigned int cnt = 0;
    static unsigned int piv_sum = 0;
    static int init = 0;

    /* reset counter and clear status flag (interrupt) */
    DEV_at91_PIT_get_PIVR (&picnt);

    /* detect missed ticks interrupts */
    if (picnt > 1)
    {
        if (init == 0)
        {
            /* throw away the first value because the counter is not initialized */
            init = 1;
            picnt = 1;
        }
        else
        {
            ovr_cnt++;
            pit_overrun += (picnt - 1);
        }
    }

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

    /* Calculate the length of the tick_interrupt,
     * if a ctx switch has occur this calculation is not correct.
     */
    piv = DEV_at91_PIT_get_PIIR (NULL);

    cnt++;
    piv_sum += piv;
    piv_av = piv_sum / cnt;
}
