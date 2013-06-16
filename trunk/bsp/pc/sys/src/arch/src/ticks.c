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
#include <dev/arch/ibmpc/ports.h>

#include <init/init.h>
#include <arch/ticks.h>
#include <arch/cpu.h>



static unsigned long
get_us (void)
{
    return 0;
}

static void pit_init(int freq)
{
   int counter = 1193182 / freq;
   outb(0x43, 0x34);
   outb(0x40,counter & 0xFF);
   outb(0x40,counter >> 8);
}
extern void
PC_ticks_init (void)
{
    USO_ticks_per_sec = PC_TICKS_IN_HZ;
    pit_init(PC_TICKS_IN_HZ);
    DEV_clock_init (get_us);
}

extern void
PC_ticks_interrupt (void)
{


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
