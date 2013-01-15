/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"
#include "uso/list.h"
#include "uso/thread.h"
#include "uso/scheduler.h"
#include "uso/sleep.h"
#include "uso/stack.h"

#include <dev/clock.h>

#define USO_SCHEDULE_TIME_MSEC 100      /* Default schedule time in msec */

long USO_schedule_time_msec = USO_SCHEDULE_TIME_MSEC;

#define PREEMPTION USO_MSEC_2_TICKS(USO_schedule_time_msec)

/*------------- Implementation ------------------------------------------*/


static USO_list_t interrupt_threads;    /* Ready queue priority Interrupt */
static USO_list_t system_threads;       /* Ready queue priority System */
static USO_list_t user_threads; /* Ready queue priority User */

static USO_thread_t idle_thread;

static USO_thread_t *current_thread = NULL;
static USO_thread_t *old_thread = NULL;

static int preemption = 0;
static unsigned long schedule_time = 0;

static void
idle (void)
{
    for (;;);
}

extern void
USO_transform (void (*init) (void), USO_stack_t * stack, int stack_size)
{
    USO_list_init (&interrupt_threads);
    USO_list_init (&system_threads);
    USO_list_init (&user_threads);
    USO_thread_init (&idle_thread, NULL, stack, stack_size, USO_IDLE, USO_FIFO, "idle");
    USO_thread_in_init (&idle_thread, NULL);
    USO_thread_out_init (&idle_thread, NULL);
    current_thread = &idle_thread;
    old_thread = &idle_thread;
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
    else if (USO_isempty (&system_threads) == FALSE)
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

static void
stack_check (void)
{
    /* For the case of an stack overflow where the sp has returned to the range between top and bottom
     * the top element is checked and if its not free, its interpreted as overrun.
     */
    if (USO_stack_check_free (old_thread->stack_top) == FALSE)
    {
        ACE_PANIC ("Stack overrun 1");
    }
    /* Also check the pointer for overrun for the case the same value as free stack
     * entries are marked was pushed to the top.
     */
    if (USO_stack_check_overrun (old_thread->stack_top, old_thread->cpu.sp) == TRUE)
    {
        ACE_PANIC ("Stack overrun 2");
    }
    if (USO_stack_check_overrun (old_thread->stack_max, old_thread->cpu.sp) == TRUE)
    {
        old_thread->stack_max = (USO_stack_t *) old_thread->cpu.sp;
    }
}

extern void
USO_schedule (USO_thread_t * new_thread)
{
    if (new_thread->stop == TRUE)
    {
        USO_thread_terminate (new_thread);
        new_thread = USO_next2run ();
    }
    /* Idle thread does not call schedule, so the old thread is terminated 
       at the next schedule call and not after the context switch! */
    if (old_thread->state == USO_EXIT)
    {
        USO_thread_terminate (old_thread);
    }
    old_thread = current_thread;
    stack_check ();
    old_thread->ticks += DEV_get_ticks_diff (schedule_time);
    schedule_time = DEV_get_ticks ();
    preemption = PREEMPTION;
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
USO_preempt (void)
{
    if ((current_thread->scheduling == USO_ROUND_ROBIN) && (--preemption <= 0))
    {
        USO_ready (current_thread);
        USO_schedule (USO_next2run ());
    }
}

extern USO_thread_t *
USO_current (void)
{
    return current_thread;
}

/*------------------------------------------------------------------------*/
