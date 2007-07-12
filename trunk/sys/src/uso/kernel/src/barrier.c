/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#include "uso/barrier.h"
#include "uso/thread.h"
#include "uso/arch/cpu.h"
#include "uso/scheduler.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_barrier_init (USO_barrier_t * barrier)
{
    USO_list_init (&barrier->threads);
}

extern void
USO_block (USO_barrier_t * barrier)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_enqueue (&barrier->threads, (USO_node_t *) USO_current());
    USO_schedule (USO_next2run ());
    USO_restore (ps);
}

extern void
USO_go (USO_barrier_t * barrier, bool_t all)
{
    USO_cpu_status_t ps = USO_disable ();
	USO_thread_t * t;
	while ( (t = (USO_thread_t *) USO_dequeue (&barrier->threads)) != NULL ){
        USO_ready (t);
        if (!all) {break;}
    }
    USO_restore (ps);
}
