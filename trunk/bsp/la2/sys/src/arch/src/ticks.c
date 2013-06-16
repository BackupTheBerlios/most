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
#include <dev/arch/at91sam7a2/csp.h>

#include <init/init.h>
#include <arch/ticks.h>
#include <arch/cpu.h>

static unsigned long
get_us (void)
{
	return 0;
}

/*
 * div_clk = (sys_clk / (2 * (syscal + 1)))
 *
 * 15 Mhz = (30 Mhz / ( 2 * 1))
 *
 * cnt_clk = (div_clk / 2^prescalar)
 *
 * (1 Mhz) 937500 hz = ( 15 MHz / 2^4 )
 *
 * 1 Mhz cnt_clk => 1us
 *
 * cnt_val(u16) = 1000000 us / LA2_TICKS_IN_HZ
 *
 * 10000 = 1000000 / 100 (100Hz)
 * 1000 =  1000000 / 1000 (1Khz)
 * 100 =   1000000 / 10000 (10Khz)
 */
extern void
LA2_ticks_init (void)
{
    unsigned int prescalar = 4; /* [3:0] */
    unsigned int syscal = 0; /* [10:0] */
    int channel = 0;
    USO_ticks_per_sec = LA2_TICKS_IN_HZ;
    CSP_STInit(ST0);
    CSP_STConfigureChannel(ST0, channel, syscal << 8 | AUTOREL | SELECTCLK_SYS | prescalar, 1000000 / USO_ticks_per_sec);
    CSP_STConfigInterrupt(ST0, (HIGH_LEVEL_SENSITIVE|PRIOR_3), channel + 1, (U32_T)LA2_ticks_interrupt);
    CSP_STEnable(ST0, channel);
    DEV_clock_init (get_us);
}

extern void
LA2_ticks_debug (void)
{
/*    ACE_printf ("Ticks interrupt time:\n"
                "\tPIV cur: %d us, av: %d us\n"
                "\tPICNT: cur: %d, ovr: %d, ovr_cnt: %d\n",
                DEV_at91_PIT_ticks_2_usec (piv),
                DEV_at91_PIT_ticks_2_usec (piv_av), picnt, pit_overrun, ovr_cnt);
*/
}

extern void
LA2_ticks_interrupt (void)
{
    ACE_u32_t status = CSP_ST_GET_SR(ST0);
    if ( (status & CHEND0) != CHEND0) return ;
    CSP_ST_SET_CSR(ST0, CHEND0);

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
