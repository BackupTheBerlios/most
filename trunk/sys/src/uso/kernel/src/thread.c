/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#include "uso/thread.h"
#include "uso/arch/cpu.h"
#include "uso/scheduler.h"
#include "uso/stack.h"
#include "uso/log.h"
#include "mfs/sysfs.h"
#include "mfs/vfs.h"
#include "mfs/descriptor.h"
#include "mfs/directory.h"

/*------------- Implementation ------------------------------------------*/

static void info(MFS_entry_t *entry);

static struct MFS_descriptor_op thread_descriptor_op = {.open = NULL,
											 .close = NULL,
										     .info = info};

static void
thread_wrapper (void)
{
    USO_current()->enter (USO_current()->arg);
    USO_exit ();
}

extern void
USO_thread_init (USO_thread_t * thread,
                 void (*enter) (void *),
                 USO_stack_t * stack,
                 int stack_size,
                 enum USO_thread_priority priority,
                 enum USO_thread_scheduling scheduling,
                 char *name)
{
    thread->cpu.pc = (USO_cpu_register_t) thread_wrapper;
    thread->cpu.ps = USO_CPU_PS_INIT;
    thread->cpu.sp = (USO_cpu_register_t) USO_stack_beginn (stack, stack_size);
    thread->enter = enter;
    thread->cleanup = NULL;
    thread->arg = NULL;
    thread->priority = priority;
    thread->scheduling = scheduling;
    thread->detach = FALSE;
    thread->free_arg = FALSE;
    thread->stop = FALSE;
    thread->in = USO_current()->in;
    thread->out = USO_current()->out;
    thread->state = USO_INIT;
    thread->stack = stack;
    thread->ticks = 0;
	thread->desc = MFS_create_unknowen(MFS_sysfs_threads(), name,
				 (MFS_entry_t*) thread, MFS_UNKNOWEN, &thread_descriptor_op);
}

extern USO_thread_t *
USO_thread_new (void (*enter) (void *),
                int stack_size,
                enum USO_thread_priority priority,
                enum USO_thread_scheduling scheduling,
                char *name,
                bool_t detach)
{
    USO_thread_t *thread = malloc (sizeof (USO_thread_t));
    if (thread){
        USO_stack_t *stack =
            malloc (stack_size * sizeof (USO_stack_t));
        if (stack) {
            USO_thread_init (thread, enter, stack, stack_size, priority, scheduling, name);
            thread->detach = detach;
        } else {
            free (thread);
            thread = NULL;        
        } 
    }
    return thread;
}

extern void
USO_thread_terminate (USO_thread_t * thread)
{
    thread->state = USO_TERMINATED;
    if (thread->cleanup != NULL) { thread->cleanup(); }
    if (thread->free_arg == TRUE) {
        free (thread->arg);
    }
    if (thread->detach == TRUE)
    {
		MFS_remove_desc(MFS_sysfs_threads(), thread->desc);
        free (thread->stack);
        free (thread);
    }
}

extern void
USO_thread_ios_init (USO_thread_t * thread,
                     FILE * in, FILE * out)
{
    thread->in = in;
    thread->out = out;
}

extern void
USO_thread_arg_init (USO_thread_t * thread, void * arg, bool_t free_arg)
{
    thread->arg = arg;
    thread->free_arg = free_arg;
}

extern void
USO_cleanup_install(void (*cleanup) (void))
{
    USO_current()->cleanup = cleanup;
}

extern void
USO_start (USO_thread_t * thread)
{
    USO_cpu_status_t ps = USO_disable ();
    if ( (thread->state == USO_INIT) || (thread->state == USO_TERMINATED) ){
		thread->stop = FALSE;
		thread->ticks = 0;
    	USO_ready (thread);
    }
    USO_restore (ps);
}

extern void
USO_stop (USO_thread_t * thread)
{
    USO_cpu_status_t ps = USO_disable ();
    if (thread->priority != USO_IDLE){
    	thread->stop = TRUE;
    }
    USO_restore (ps);
}

extern void
USO_yield (void)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_ready (USO_current());
    USO_schedule (USO_next2run ());
    USO_restore (ps);
}

extern void
USO_exit (void)
{
    USO_disable ();
    USO_current()->state = USO_EXIT;
    USO_schedule (USO_next2run ());
    /*
     * never reached 
     */
}

extern void
USO_panic (char *file, int line)
{
    USO_kprintf (USO_LL_PANIC, "\n%s: [%s:%d] panic!",
                 USO_current()->desc->name, file, line);
    USO_disable ();
    for (;;);
}

static void
info(MFS_entry_t *entry)
{
	USO_thread_t *thread = (USO_thread_t *)entry;
    char *priority, *scheduling, *state, *error;
    error = "error";
    
    switch (thread->priority)
    {
    case USO_IDLE:
        priority = "idle";
        break;
    case USO_SYSTEM:
        priority = "sys";
        break;
    case USO_USER:
        priority = "user";
        break;
    case USO_INTERRUPT:
        priority = "int";
        break;
    default:
        priority = error;
        break;
    }
    
    switch (thread->scheduling)
    {
    case USO_FIFO:
        scheduling = "fifo";
        break;
    case USO_ROUND_ROBIN:
        scheduling = "roro";
        break;
    default:
        scheduling = error;
        break;
    }
    
    switch (thread->state)
    {
    case USO_INIT:
        state = "init";
        break;
    case USO_RUNNING:
        state = "running";
        break;
    case USO_READY:
        state = "ready";
        break;
    case USO_BLOCKED_WAIT:
        state = "b_wait";
        break;
    case USO_BLOCKED_MONITOR:
        state = "b_monitor";
        break;
    case USO_BLOCKED_SLEEP:
        state = "b_sleep";
        break;
    case USO_BLOCKED_SEND:
        state = "b_send";
        break;
    case USO_BLOCKED_RECEIVE:
        state = "b_receive";
        break;
    case USO_BLOCKED_REPLY:
        state = "b_reply";
        break;
    case USO_EXIT:
        state = "exit";
        break;
    case USO_TERMINATED:
        state = "terminated";
        break;
    default:
        state = error;
        break;
    }
    
    printf ("%s\t%s\t%s\t%lu\n", priority, scheduling, state, thread->ticks);
}

/*------------------------------------------------------------------------*/
