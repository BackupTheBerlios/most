/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/thread.h"
#include "uso/arch/cpu.h"
#include "uso/scheduler.h"
#include "uso/heap.h"
#include "uso/monitor.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_monitor (USO_mutex_t * mutex, USO_list_t * condition)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_unlock(mutex);
    USO_current()->state = USO_BLOCKED_MONITOR;
    USO_enqueue (condition, (USO_node_t *) USO_current());
    USO_schedule (USO_next2run ());
    USO_lock(mutex);
    USO_restore (ps);
}

extern void
USO_notify (USO_list_t * condition)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t * thread;
    if ( (thread = (USO_thread_t *) USO_dequeue (condition)) != NULL){
      USO_ready (thread);
    }
    USO_restore (ps);
}

extern void
USO_notify_all (USO_list_t * condition)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t * thread;
    while ( (thread = (USO_thread_t *) USO_dequeue (condition)) != NULL){
      USO_ready (thread);
    }
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
