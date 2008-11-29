/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/scheduler.h"
#include "uso/mutex.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_mutex_init (USO_mutex_t * mutex)
{
    mutex->lock = NULL;
    USO_list_init (&mutex->threads);
}

extern void
USO_lock (USO_mutex_t * mutex)
{
    USO_cpu_status_t ps = USO_disable ();
	USO_thread_t* current = USO_current();
    if (mutex->lock == NULL)
    {
    	mutex->lock = current;	
    }
    else if (mutex->lock != current)
    {
        current->state = USO_BLOCKED_LOCK;
        USO_enqueue (&mutex->threads, (USO_node_t *) current);
        USO_schedule (USO_next2run ());
    }
    USO_restore (ps);
}

extern void
USO_unlock (USO_mutex_t * mutex)
{
    USO_cpu_status_t ps = USO_disable ();
	mutex->lock = NULL;
	USO_thread_t * t;
	if ( (t = (USO_thread_t *) USO_dequeue (&mutex->threads)) != NULL ){
        USO_ready (t);
    }
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
