/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/semaphore.h"
#include "uso/thread.h"
#include "uso/arch/cpu.h"
#include "uso/scheduler.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_semaphore_init (USO_semaphore_t * semaphore, int count)
{
    semaphore->count = count;
    USO_list_init (&semaphore->threads);
}

extern void
USO_wait (USO_semaphore_t * semaphore)
{
    USO_cpu_status_t ps = USO_disable ();
    if (--(semaphore->count) < 0)
    {
        USO_current()->state = USO_BLOCKED_WAIT;
        USO_enqueue (&semaphore->threads, (USO_node_t *) USO_current());
        USO_schedule (USO_next2run ());
    }
    USO_restore (ps);
}

extern void
USO_signal (USO_semaphore_t * semaphore)
{
    USO_cpu_status_t ps = USO_disable ();
    if ((semaphore->count)++ < 0)
    {
        USO_ready ((USO_thread_t *) USO_dequeue (&semaphore->threads));
    }
    USO_restore (ps);
}

extern int
USO_sem_count (USO_semaphore_t * semaphore)
{
    return semaphore->count;
}

/*------------------------------------------------------------------------*/
