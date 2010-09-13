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
    thread->stop = FALSE;
    thread->flags = 0;
    thread->signals = 0;
    thread->in = USO_current()->in;
    thread->out = USO_current()->out;
    thread->state = USO_INIT;
    thread->stack = stack;
    thread->stack_size = stack_size;
    thread->stack_bot = USO_stack_beginn (stack, stack_size);
    thread->stack_top = USO_stack_end (stack, stack_size);
    thread->stack_max = USO_stack_beginn (stack, stack_size);
    thread->ticks = 0;
	thread->desc = MFS_create_desc(MFS_sysfs_threads(), name,
				 (MFS_entry_t*) thread, MFS_DESC, &thread_descriptor_op);
    if (priority != USO_IDLE){
    	USO_stack_init(stack, stack_size);
    }
}

extern USO_thread_t *
USO_thread_new (void (*enter) (void *),
                int stack_size,
                enum USO_thread_priority priority,
                enum USO_thread_scheduling scheduling,
                char *name)
{
    USO_thread_t *thread = ACE_malloc (sizeof (USO_thread_t));
    if (thread){
        USO_stack_t *stack =
            ACE_malloc (stack_size * sizeof (USO_stack_t));
        if (stack) {
            USO_thread_init (thread, enter, stack, stack_size, priority, scheduling, name);
        } else {
            ACE_free (thread);
            thread = NULL;        
        } 
    }
    return thread;
}

extern void
USO_thread_terminate (USO_thread_t * thread)
{
    thread->state = USO_DEAD;
    if (thread->cleanup != NULL) { thread->cleanup(); }
    if ( (thread->flags & (1 << USO_FLAG_FREE_ARG)) == (1 << USO_FLAG_FREE_ARG))
    {
        ACE_free (thread->arg);
    }
    if ( (thread->flags & (1 << USO_FLAG_DETACH)) == (1 << USO_FLAG_DETACH))
    {
		MFS_remove_desc(MFS_sysfs_threads(), thread->desc);
        ACE_free (thread->stack);
        ACE_free (thread);
    }
}

extern void
USO_thread_ios_init (USO_thread_t * thread,
                     ACE_FILE * in, ACE_FILE * out)
{
    thread->in = in;
    thread->out = out;
}

extern void
USO_thread_arg_init (USO_thread_t * thread, void * arg)
{
    thread->arg = arg;
}

extern void
USO_thread_flags_set(USO_thread_t * thread, ACE_u32_t flags)
{
	thread->flags |= flags;
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
    if ( (thread->state == USO_INIT) || (thread->state == USO_DEAD) ){
		thread->stop = FALSE;
		thread->ticks = 0;
	    thread->signals = 0;
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
		if (thread->state == USO_BLOCKED_CATCH)
		{
			USO_ready (thread);
		}
    }
    USO_restore (ps);
}

extern void
USO_raise(USO_thread_t * thread, ACE_u32_t signals)
{
    USO_cpu_status_t ps = USO_disable ();
	thread->signals |= signals;
	if (thread->state == USO_BLOCKED_CATCH)
	{
		USO_ready (thread);
	}
    USO_restore (ps);
}

extern ACE_u32_t
USO_catch(void)
{
	ACE_u32_t signals;
    USO_cpu_status_t ps = USO_disable ();
	USO_thread_t * current = USO_current();
	while (current->signals == 0)
	{
	    current->state = USO_BLOCKED_CATCH;
	    USO_schedule (USO_next2run ());
	}
	signals = current->signals;
	current->signals = 0;
    USO_restore (ps);
    return signals;
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
        scheduling = "fi";
        break;
    case USO_ROUND_ROBIN:
        scheduling = "ro";
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
    case USO_BLOCKED_BLOCK:
        state = "b_block";
        break;
    case USO_BLOCKED_LOCK:
        state = "b_lock";
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
    case USO_BLOCKED_CATCH:
        state = "b_catch";
        break;
    case USO_EXIT:
        state = "exit";
        break;
    case USO_DEAD:
        state = "dead";
        break;
    default:
        state = error;
        break;
    }
    
    ACE_printf ("%s\t%s\t%s\t%lu\t%i\t%i\t%p %p %p %p\n",
    	priority,
    	scheduling,
    	state,
    	thread->ticks,
    	thread->stack_size * sizeof(USO_stack_t),
    	USO_stack_get_free(thread->stack_top, thread->stack_size) * sizeof(USO_stack_t),
    	(void*)thread->stack_top,
    	(void*)thread->stack_max,
    	(void*)thread->cpu.sp,
    	(void*)thread->stack_bot);
}

extern void
USO_thread_info_head(void)
{
	ACE_printf ("\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		"Name",
		"Prior",
		"Sched",
		"State",
		"Ticks",
		"S.size",
		"S.free",
		"S.top",
		"S.max",
		"S.sp",
		"S.bot");
}
/*------------------------------------------------------------------------*/
