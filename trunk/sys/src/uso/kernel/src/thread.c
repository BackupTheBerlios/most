/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#include <uso/thread.h>
#include <uso/cpu.h>
#include <uso/scheduler.h>
#include <uso/stack.h>
#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/stream.h>



static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    USO_thread_t *thread = (USO_thread_t *) desc->represent;

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

    
    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "Prior";
            entry->value.s = priority;
            break;
        case 1:
            entry->type = MFS_INFO_STRING;
            entry->name = "Sched";
            entry->value.s = scheduling;
            break;
        case 2:
            entry->type = MFS_INFO_STRING;
            entry->name = "State";
            entry->value.s = state;
            break;
        case 3:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Ticks";
            entry->value.z = thread->ticks;
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "Error";
            entry->value.l = thread->error;
            break;
        case 5:
            entry->type = MFS_INFO_SIZE;
            entry->name = "S.size";
            entry->value.z = thread->stack_size * sizeof (USO_stack_t);
            break;
        case 6:
            entry->type = MFS_INFO_SIZE;
            entry->name = "S.free";
            entry->value.z = USO_stack_get_free (thread->stack_top, thread->stack_size) * sizeof (USO_stack_t);
            break;
        case 7:
            entry->type = MFS_INFO_PTR;
            entry->name = "S.top";
            entry->value.p = (void *)thread->stack_top;
            break;
        case 8:
            entry->type = MFS_INFO_PTR;
            entry->name = "S.max";
            entry->value.p = (void *)thread->stack_max;
            break;
        case 9:
            entry->type = MFS_INFO_PTR;
            entry->name = "S.sp";
            entry->value.p = (void *)thread->cpu.sp;
            break;
        case 10:
            entry->type = MFS_INFO_PTR;
            entry->name = "S.bot";
            entry->value.p = (void *)thread->stack_bot;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static ACE_err_t delete (MFS_descriptor_t *desc)
{
    USO_thread_t *thread = (USO_thread_t *) desc->represent;
    if (thread->state == USO_INIT || thread->state == USO_DEAD){
        return ACE_OK;
    }
    return DEF_ERR_IN_USE;
}

static struct MFS_descriptor_op thread_descriptor_op = {
                .open = NULL,
                .close = NULL,
                .info = info,
                .control = NULL,
                .delete = delete
};

static void
thread_wrapper (void)
{
    USO_thread_t *t = USO_current();
    USO_exit (t->enter (t->arg));
}

extern void
USO_thread_init (USO_thread_t * thread,
                 ACE_err_t (*enter) (void *),
                 USO_stack_t * stack,
                 int stack_size,
                 enum USO_thread_priority priority,
                 enum USO_thread_scheduling scheduling, char *name)
{
    thread->cpu.pc = (USO_cpu_register_t) thread_wrapper;
    thread->cpu.ps = USO_CPU_PS_INIT;
    thread->cpu.sp = (USO_cpu_register_t) USO_stack_beginn (stack, stack_size);
    thread->enter = enter;
    thread->cleanup = NULL;
    thread->arg = NULL;
    thread->priority = priority;
    thread->scheduling = scheduling;
    thread->state = USO_INIT;
    thread->flags = 0;
    thread->signals = 0;
    thread->stack = stack;
    thread->stack_size = stack_size;
    thread->stack_bot = USO_stack_beginn (stack, stack_size);
    thread->stack_top = USO_stack_end (stack, stack_size);
    thread->stack_max = USO_stack_beginn (stack, stack_size);
    thread->ticks = 0;
    thread->in = USO_current ()->in;
    thread->out = USO_current ()->out;
    thread->work = USO_current ()->work;
    thread->error = ACE_OK;
    MFS_descriptor_t *dir = MFS_resolve("/sys/uso/thread");
    thread->desc = MFS_descriptor_create (dir, name, MFS_SYS, &thread_descriptor_op, (MFS_represent_t *) thread);
    MFS_close_desc(dir);
    if (priority != USO_IDLE)
    {
        USO_stack_init (stack, stack_size);
    }
}

extern USO_thread_t *
USO_thread_new (ACE_err_t (*enter) (void *),
                int stack_size,
                enum USO_thread_priority priority,
                enum USO_thread_scheduling scheduling, char *name)
{
    USO_thread_t *thread = ACE_malloc (sizeof (USO_thread_t));
    if (thread)
    {
        USO_stack_t *stack = ACE_malloc (stack_size * sizeof (USO_stack_t));
        if (stack)
        {
            USO_thread_init (thread, enter, stack, stack_size, priority, scheduling, name);
        }
        else
        {
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
    if (thread->cleanup != NULL)
    {
        thread->cleanup (thread->arg);
    }
    if ((thread->flags & (1 << USO_FLAG_FREE_ARG)) == (1 << USO_FLAG_FREE_ARG))
    {
        ACE_free (thread->arg);
    }
    if ((thread->flags & (1 << USO_FLAG_CLOSE_IN)) == (1 << USO_FLAG_CLOSE_IN))
    {
        MFS_close_desc(thread->in);
    }
    if ((thread->flags & (1 << USO_FLAG_CLOSE_OUT)) == (1 << USO_FLAG_CLOSE_OUT))
    {
        MFS_close_desc(thread->out);
    }
    if ((thread->flags & (1 << USO_FLAG_DETACH)) == (1 << USO_FLAG_DETACH))
    {
        MFS_descriptor_t *dir = MFS_resolve("/sys/uso/thread");
        MFS_remove_desc (dir, thread->desc);
        MFS_close_desc(dir);
        ACE_free (thread->stack);
        ACE_free (thread);
    }
}

extern void
USO_thread_in_init (USO_thread_t * thread, MFS_descriptor_t * in)
{
    thread->in = in;
}

extern void
USO_thread_out_init (USO_thread_t * thread, MFS_descriptor_t * out)
{
    thread->out = out;
}

extern void
USO_thread_arg_init (USO_thread_t * thread, void *arg)
{
    thread->arg = arg;
}

extern void
USO_thread_work_set (USO_thread_t * thread, MFS_descriptor_t * work)
{
    thread->work = work;
}

extern MFS_descriptor_t *
USO_thread_work_get (USO_thread_t * thread)
{
    return thread->work;
}

extern void
USO_thread_flags_set (USO_thread_t * thread, ACE_u32_t flags)
{
    thread->flags |= flags;
}

extern void
USO_cleanup_install (void (*cleanup) (void *))
{
    USO_current ()->cleanup = cleanup;
}

extern void
USO_start (USO_thread_t * thread)
{
    USO_cpu_status_t ps = USO_disable ();
    if ((thread->state == USO_INIT) || (thread->state == USO_DEAD))
    {
        thread->ticks = 0;
        thread->signals = 0;
        USO_ready (thread);
    }
    USO_restore (ps);
}

extern void
USO_stop (USO_thread_t * thread)
{
    USO_raise(thread, USO_SIGNAL_STOP);
}

extern void
USO_raise (USO_thread_t * thread, ACE_u32_t signal)
{
    USO_cpu_status_t ps = USO_disable ();
    thread->signals |= (1 << signal);
    if (thread->state == USO_BLOCKED_CATCH)
    {
        USO_ready (thread);
    }
    USO_restore (ps);
}

extern ACE_u32_t
USO_catch (void)
{
    ACE_u32_t signals;
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t *current = USO_current ();
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
    USO_ready (USO_current ());
    USO_schedule (USO_next2run ());
    USO_restore (ps);
}

extern void
USO_exit (ACE_err_t err)
{
    USO_thread_t *t = USO_current();
    USO_disable ();
    t->error = err;
    t->state = USO_EXIT;
    USO_schedule (USO_next2run ());
    /*
     * never reached 
     */
}

extern char *
USO_thread_name (void)
{
    return USO_current ()->desc->name;
}

