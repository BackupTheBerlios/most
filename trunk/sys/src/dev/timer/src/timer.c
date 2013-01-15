/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/list.h>
#include <uso/semaphore.h>
#include <uso/cpu.h>
#include <uso/heap.h>
#include <uso/thread.h>
#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include "dev/timer.h"


/*------------- Implementation ------------------------------------------*/

static USO_list_t onTimer;
static USO_list_t elapsedTimer;
static USO_semaphore_t cond;

static USO_thread_t *timers_thread = NULL;

static void
info (MFS_descriptor_t * desc)
{
    DEV_timer_t *timer = (DEV_timer_t *) desc->represent;
    ACE_printf ("Timer state: %s , ticks: %lu, ctx: %s\n",
                timer->state == DEV_TIMER_ON ? "ON" : "OFF",
                timer->ticks, timer->ctx == DEV_TIMER_INT ? "INT" : "THREAD");
}

static struct MFS_descriptor_op timer_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
};

static void
produce (DEV_timer_t * timer)
{
    USO_enqueue (&elapsedTimer, (USO_node_t *) timer);
    USO_signal (&cond);
}

static DEV_timer_t *
consume (void)
{
    DEV_timer_t *timer;
    USO_cpu_status_t ps = USO_disable ();
    while ((timer = (DEV_timer_t *) USO_dequeue (&elapsedTimer)) == NULL)
    {
        USO_restore (ps);
        USO_wait (&cond);
        ps = USO_disable ();
    }
    USO_restore (ps);
    return timer;
}

static void
timers_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Timers is running.\n");
    for (;;)
    {
        DEV_timer_t *timer = consume ();
        timer->f (timer->param);
    }
}

extern void
DEV_timers_start (int timers_stack_size)
{
    timers_thread = USO_thread_new (timers_run,
                                    timers_stack_size, USO_INTERRUPT, USO_FIFO, "timers");
    USO_start (timers_thread);
}



extern void
DEV_timers_init (void)
{
    USO_list_init (&onTimer);
    USO_list_init (&elapsedTimer);
    USO_semaphore_init (&cond, 0);
}

extern void
DEV_timer_init (DEV_timer_t * timer, void (*f) (void *), void *param, enum DEV_timer_ctx ctx)
{
    timer->f = f;
    timer->param = param;
    timer->state = DEV_TIMER_OFF;
    timer->ctx = ctx;
}

extern void
DEV_timer_install (DEV_timer_t * timer, char *name)
{
    timer->desc = MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/dev/clock/timer"), name,
                                    MFS_SYS, &timer_descriptor_op, (MFS_represent_t *) timer);
}

extern void
DEV_timer_remove (DEV_timer_t * timer)
{
    MFS_remove_desc (MFS_resolve(MFS_get_root(), "sys/dev/clock/timer"), timer->desc);
}

extern void
DEV_timer_start (DEV_timer_t * timer, long ticks)
{
    USO_cpu_status_t ps = USO_disable ();
    if (timer->state == DEV_TIMER_ON)
    {
        DEV_timer_stop (timer);
    }
    timer->ticks = ticks;
    USO_delta_insert (&onTimer, (USO_node_t *) timer, timer->ticks);
    timer->state = DEV_TIMER_ON;
    USO_restore (ps);
}

extern void
DEV_timer_stop (DEV_timer_t * timer)
{
    USO_cpu_status_t ps = USO_disable ();
    if (timer->state == DEV_TIMER_ON)
    {
        USO_delta_remove (&onTimer, (USO_node_t *) timer);
        timer->state = DEV_TIMER_OFF;
    }
    USO_restore (ps);
}

extern void
DEV_timer_fire (void)
{
    USO_delta_dec (&onTimer);
    while (USO_delta_null (&onTimer) == TRUE)
    {
        DEV_timer_t *timer = (DEV_timer_t *) USO_pop (&onTimer);
        timer->state = DEV_TIMER_OFF;
        if (timer->ctx == DEV_TIMER_INT)
            timer->f (timer->param);
        else
            produce (timer);
    }
}


/*------------------------------------------------------------------------*/
