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
#include <dev/arch/at91sam7x/AT91SAM7X256.h>
#include <dev/arch/at91sam7x/pit.h>

#include "init/init.h"
#include "arch/ticks.h"
#include "arch/cpu.h"

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
    DEV_at91_PIT_init (DEV_AT91_PIT_SEC_IN_USEC / SAM_TICKS_IN_HZ, SAM_MCK_IN_MHZ);
    DEV_at91_PIT_enable_IT ();
    DEV_clock_init (get_us);
}

extern void
SAM_ticks_debug (void)
{
    ACE_printf ("Ticks interrupt time:\n"
                "\tPICNT: cur: %d, ovr: %d, ovr_cnt: %d\n",
                picnt, pit_overrun, ovr_cnt);
}

extern void
SAM_ticks_interrupt (void)
{
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

}
