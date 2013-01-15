/*
 * events.c
 *
 *  Created on: 27.04.2010
 *      Author: maik
 */

#include <uso/event.h>
#include "init/events.h"
#include "arch/digio.h"

#define EVENT_BUFS 5

USO_list_t SAM_event_slots;

struct events
{
    DEV_digin_t *in;
    enum SAM_event pressed;
    enum SAM_event not_pressed;
};

USO_buf_pool_t SAM_event_pool;
static USO_node_t event_buffers[EVENT_BUFS];

static const struct events ev[] = {
    {&SAM_switch_1, SAM_switch_1_pressed, SAM_switch_1_not_pressed},
    {&SAM_switch_2, SAM_switch_2_pressed, SAM_switch_2_not_pressed},
    {&SAM_card_insert, SAM_card_inserted, SAM_card_removed},
    {&SAM_joystick_up, SAM_joystick_up_pressed, SAM_joystick_up_not_pressed},
    {&SAM_joystick_down, SAM_joystick_down_pressed, SAM_joystick_down_not_pressed},
    {&SAM_joystick_left, SAM_joystick_left_pressed, SAM_joystick_left_not_pressed},
    {&SAM_joystick_right, SAM_joystick_right_pressed, SAM_joystick_right_not_pressed},
    {&SAM_joystick_center, SAM_joystick_center_pressed, SAM_joystick_center_not_pressed},
    {NULL, 0, 0}
};

extern void
SAM_events_init (void)
{
    USO_list_init (&SAM_event_slots);
    USO_buf_pool_init (&SAM_event_pool, event_buffers, EVENT_BUFS, sizeof (USO_node_t));
}

extern void
SAM_event_create (enum SAM_event event)
{
    USO_node_t *e = USO_buf_alloc (&SAM_event_pool);
    e->data = event;
    if (FALSE == USO_slots_put_event (&SAM_event_slots, NULL, e, FALSE))
        USO_buf_free (&SAM_event_pool, e);
}

extern void
SAM_events_create (void)
{
    DEV_diginputs_sample (&SAM_control_in);
    for (int i = 0; ev[i].in != NULL; ++i)
    {
        enum DEV_digio_edge edge = DEV_digin_ischanged (ev[i].in);
        enum SAM_event event;
        if (edge != DEV_DIGIO_NO)
        {
            if (edge == DEV_DIGIO_RAISE)
                event = ev[i].pressed;
            else
                event = ev[i].not_pressed;
            SAM_event_create (event);
        }
    }
}
