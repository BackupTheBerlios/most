/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>

#include "uso/list.h"
#include "uso/thread.h"
#include "uso/scheduler.h"
#include "uso/sleep.h"

#include <dev/clock.h>

/*------------- Implementation ------------------------------------------*/

#define PREEMPTION ACE_MSEC_2_TICKS(USO_SCHEDULE_TIME_MSEC)

static USO_list_t interrupt_threads;    /* Ready queue priority Interrupt */
static USO_list_t system_threads;       /* Ready queue priority System */
static USO_list_t user_threads;         /* Ready queue priority User */

static USO_thread_t idle_thread;

static USO_thread_t *current_thread = NULL;
static USO_thread_t *old_thread = NULL;

static int preemption;
static unsigned long schedule_time;

static void
idle (void)
{
    for (;;);
}

extern void
USO_transform (void (*init) (void))
{
    USO_list_init (&interrupt_threads);
    USO_list_init (&system_threads);
    USO_list_init (&user_threads);
    USO_thread_init (&idle_thread, NULL, NULL, 0, USO_IDLE, USO_FIFO, "Idle");
    USO_thread_ios_init (&idle_thread, NULL, NULL);
    current_thread = &idle_thread;
    old_thread = &idle_thread;
    USO_sleep_init ();
    ACE_stdio_init ();
    init ();
    idle ();
}

extern USO_thread_t *
USO_next2run (void)
{
    if (USO_isempty (&interrupt_threads) == FALSE)
    {
        return (USO_thread_t *) USO_dequeue (&interrupt_threads);
    }
    if (USO_isempty (&system_threads) == FALSE)
    {
        return (USO_thread_t *) USO_dequeue (&system_threads);
    }
    else if (USO_isempty (&user_threads) == FALSE)
    {
        return (USO_thread_t *) USO_dequeue (&user_threads);
    }
    else
    {
        return &idle_thread;
    }
}

extern void
USO_schedule (USO_thread_t * new_thread)
{
    if (new_thread->stop == TRUE)
    {
        USO_thread_terminate (new_thread);
        new_thread = USO_next2run();
    }
    /* Idle thread does not call schedule, so the old thread is terminared 
       at the next schedule call and not after the context switch! */
    if (old_thread->state == USO_EXIT)
    {
        USO_thread_terminate (old_thread);
    }
    old_thread = current_thread;
    old_thread->ticks += DEV_get_ticks_diff(schedule_time);
    preemption = PREEMPTION;
    schedule_time = DEV_get_ticks();
    new_thread->state = USO_RUNNING;
    current_thread = new_thread;
    USO_context_switch (&old_thread->cpu, &new_thread->cpu);
}

extern void
USO_ready (USO_thread_t * thread)
{
    thread->state = USO_READY;
	if (thread->priority == USO_INTERRUPT)
    {
        USO_enqueue (&interrupt_threads, (USO_node_t *) thread);
	    if (current_thread->priority < USO_INTERRUPT)
    	{
        	USO_ready (current_thread);
        	USO_schedule (USO_next2run ());
    	}
    	return;
    }
	if (thread->priority == USO_SYSTEM)
    {
        USO_enqueue (&system_threads, (USO_node_t *) thread);
    }
    else if (thread->priority == USO_USER)
    {
        USO_enqueue (&user_threads, (USO_node_t *) thread);
    }
    else
    {
        return;
    }
    if (current_thread->priority == USO_IDLE)
    {
        USO_ready (current_thread);
        USO_schedule (USO_next2run ());
    }
}

extern void
USO_preempt(void)
{
    if ( (current_thread->scheduling == USO_ROUND_ROBIN) && (--preemption <= 0) ){
        USO_ready (current_thread);
        USO_schedule (USO_next2run ());
    }
}

extern USO_thread_t * 
USO_current(void)
{
    return current_thread;
}

/*------------------------------------------------------------------------*/
