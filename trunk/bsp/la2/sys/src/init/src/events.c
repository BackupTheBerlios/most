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

USO_list_t LA2_event_slots;

struct events
{
    DEV_digin_t *in;
    enum LA2_event pressed;
    enum LA2_event released;
};

USO_buf_pool_t LA2_event_pool;
static USO_node_t event_buffers[EVENT_BUFS];

static const struct events ev[] = {
    {&LA2_card_insert, LA2_card_inserted, LA2_card_removed},
    {NULL, 0, 0}
};

extern void
LA2_events_init (void)
{
    USO_list_init (&LA2_event_slots);
    USO_buf_pool_init (&LA2_event_pool, event_buffers, EVENT_BUFS, sizeof (USO_node_t));
}

extern void
LA2_event_create (enum LA2_event event)
{
    USO_node_t *e = USO_buf_alloc (&LA2_event_pool);
    e->data = event;
    if (FALSE == USO_slots_put_event (&LA2_event_slots, NULL, e, FALSE))
        USO_buf_free (&LA2_event_pool, e);
}

extern void
LA2_events_generate (void)
{
    DEV_diginputs_sample (&LA2_control_in);
    for (int i = 0; ev[i].in != NULL; ++i)
    {
        enum DEV_digio_edge edge = DEV_digin_ischanged (ev[i].in);
        enum LA2_event event;
        if (edge != DEV_DIGIO_NO)
        {
            if (edge == DEV_DIGIO_RAISE)
                event = ev[i].pressed;
            else
                event = ev[i].released;
            LA2_event_create (event);
        }
    }
}
