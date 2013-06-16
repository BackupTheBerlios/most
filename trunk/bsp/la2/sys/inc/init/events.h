/*
 * events.h
 *
 *  Created on: 27.04.2010
 *      Author: maik
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include <uso/list.h>
#include <uso/buf_pool.h>

/** @addtogroup init
 *
 * @{
 */

/** @defgroup events events.h
 *
 * SAM Events
 *
 * @{
 */

#define LA2_EVENT_BASE          1    /**< Max 1000 basis events. */

/** SAM Events */
enum LA2_event
{
    LA2_card_inserted = LA2_EVENT_BASE,
    LA2_card_removed
};

extern USO_list_t LA2_event_slots;
extern USO_buf_pool_t LA2_event_pool;

/**
 */
extern void LA2_events_init (void);

/**
 */
extern void LA2_event_create (enum LA2_event event);

/**
 */
extern void LA2_events_generate (void);

/** @}
 */

/** @}
 */

#endif /* EVENTS_H_ */
