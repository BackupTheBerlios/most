/*
 * event.c
 *
 *  Created on: 27.04.2010
 *      Author: maik
 */

#include "uso/event.h"

static ACE_bool_t
find_id (const long id_list[], long id)
{
    for (int i = 0; id_list[i] != 0; ++i)
    {
        if (id_list[i] == id)
            return TRUE;
    }
    return FALSE;
}

static ACE_bool_t
slot_put_event (USO_slot_t * slot, USO_node_t * event, ACE_bool_t block)
{
    if (TRUE == find_id (slot->id_list, event->data))
    {
        if (block == TRUE)
            if (USO_post_would_block (&slot->mbox) <= 0)
                return FALSE;
        USO_post (&slot->mbox, event);
        return TRUE;
    }
    return FALSE;
}

/* Not thread save, have to be called not concurrently. */
extern void
USO_slot_init (USO_list_t * slots, USO_slot_t * slot, const long id_list[], int size)
{
    USO_mailbox_init (&slot->mbox, size);
    slot->id_list = id_list;
    USO_enqueue (slots, (USO_node_t *) slot);
}

extern USO_node_t *
USO_slot_get_event (USO_slot_t * slot)
{
    return USO_fetch (&slot->mbox);
}

extern USO_node_t *
USO_slot_get_event_nb (USO_slot_t * slot)
{
    if (USO_fetch_would_block (&slot->mbox) <= 0)
        return NULL;
    return USO_slot_get_event (slot);
}

extern ACE_bool_t
USO_slots_put_event (USO_list_t * slots, USO_slot_t * iterator, USO_node_t * event,
                     ACE_bool_t block)
{
    while (NULL != (iterator = (USO_slot_t *) USO_next_element (slots, (USO_node_t *) iterator)))
    {
        if (TRUE == slot_put_event (iterator, event, block))
            return TRUE;
    }
    return FALSE;
}
