/*
 * event.h
 *
 *  Created on: 27.04.2010
 *      Author: maik
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "uso/list.h"
#include "uso/mailbox.h"

/** @addtogroup uso
 * @{
 */

/** @defgroup event event.h
 *
 * A thread will block for a amount of time.
 *
 * @{
 */


struct PRI_slot
{
    USO_node_t node;
    USO_mailbox_t mbox;
    const long *id_list;
};

/**
 *
 */
typedef struct PRI_slot USO_slot_t;

/**
 *
 */
extern void USO_slot_init (USO_list_t * slots, USO_slot_t * slot, const long id_list[], int size);

/**
 *
 */
extern USO_node_t *USO_slot_get_event (USO_slot_t * slot);

/**
 *
 */
extern USO_node_t *USO_slot_get_event_nb (USO_slot_t * slot);

/**
 *
 */
extern ACE_bool_t USO_slots_put_event (USO_list_t * slots, USO_slot_t * iterator,
                                       USO_node_t * event, ACE_bool_t block);


/** @}
 */

/** @}
 */

#endif /* EVENT_H_ */
