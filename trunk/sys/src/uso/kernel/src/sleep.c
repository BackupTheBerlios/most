/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/sleep.h"
#include "uso/thread.h"
#include "uso/list.h"
#include "uso/arch/cpu.h"
#include "uso/scheduler.h"

/*------------- Implementation ------------------------------------------*/

long USO_ticks_per_sec = USO_TICKS_1KHZ;

static USO_list_t sleepingThreads;

extern void
USO_sleep_init (void)
{
    USO_list_init (&sleepingThreads);
}

extern void
USO_wakeup (void)
{
    // no sync because its just called from interrupt(each tick). 
    USO_delta_dec (&sleepingThreads);
    while (USO_delta_null (&sleepingThreads) == TRUE)
    {
        USO_ready ((USO_thread_t *) USO_pop (&sleepingThreads));
    }
}

extern void
USO_sleep (long ticks)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t *current = USO_current ();
    current->state = USO_BLOCKED_SLEEP;
    USO_delta_insert (&sleepingThreads, (USO_node_t *) current, ticks);
    USO_schedule (USO_next2run ());
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
