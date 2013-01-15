/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/thread.h"
#include "uso/cpu.h"
#include "uso/scheduler.h"
#include "uso/heap.h"
#include "uso/monitor.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_monitor (USO_mutex_t * mutex, USO_barrier_t * barrier)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_unlock (mutex);
    USO_block (barrier);
    USO_lock (mutex);
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
