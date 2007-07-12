/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/list.h>
#include <uso/arch/cpu.h>
#include <uso/heap.h>

#include "dev/timer.h"

/*------------- Implementation ------------------------------------------*/

static USO_list_t onTimer;

extern void
DEV_timers_init (void)
{
    USO_list_init (&onTimer);
}

extern void
DEV_timer_init (DEV_timer_t * timer,
                void (*f) (void *), void *param, long ticks)
{
    timer->ticks = ticks;
    timer->f = f;
    timer->param = param;
    timer->state = DEV_TIMER_OFF;
}

extern void
DEV_timer_start (DEV_timer_t * timer)
{
    USO_cpu_status_t ps = USO_disable ();
    if (timer->state == DEV_TIMER_ON)
    {
        DEV_timer_stop (timer);
    }
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
        timer->f (timer->param);
    }
}

/*------------------------------------------------------------------------*/
