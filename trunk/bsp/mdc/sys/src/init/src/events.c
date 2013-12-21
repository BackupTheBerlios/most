/*
 * events.c
 *
 *  Created on: 27.04.2010
 *      Author: maik
 */

#include <uso/event.h>
#include <init/events.h>
#include <arch/digio.h>

#define EVENT_BUFS 5

USO_list_t MDC_event_slots;

struct events
{
    DEV_digin_t *in;
    enum MDC_event pressed;
    enum MDC_event released;
};

USO_buf_pool_t MDC_event_pool;
static USO_node_t event_buffers[EVENT_BUFS];

static const struct events ev[] = {
    {&MDC_red, MDC_red_pressed, MDC_red_released},
    {&MDC_green, MDC_green_pressed, MDC_green_released},
    {&MDC_blue, MDC_blue_pressed, MDC_blue_released},
    {&MDC_yellow, MDC_yellow_pressed, MDC_yellow_released},
    {NULL, 0, 0}
};

extern void
MDC_events_init (void)
{
    USO_list_init (&MDC_event_slots);
    USO_buf_pool_init (&MDC_event_pool, event_buffers, EVENT_BUFS, sizeof (USO_node_t));
}

extern void
MDC_event_create (enum MDC_event event)
{
    USO_node_t *e = USO_buf_alloc (&MDC_event_pool);
    e->data = event;
    if (FALSE == USO_slots_put_event (&MDC_event_slots, NULL, e, FALSE))
        USO_buf_free (&MDC_event_pool, e);
}

extern void
MDC_events_create (void)
{
    DEV_diginputs_sample (&MDC_control_in);
    for (int i = 0; ev[i].in != NULL; ++i)
    {
        enum DEV_digio_edge edge = DEV_digin_ischanged (ev[i].in);
        enum MDC_event event;
        if (edge != DEV_DIGIO_NO)
        {
            if (edge == DEV_DIGIO_RAISE)
                event = ev[i].pressed;
            else
                event = ev[i].released;
            MDC_event_create (event);
        }
    }
}
