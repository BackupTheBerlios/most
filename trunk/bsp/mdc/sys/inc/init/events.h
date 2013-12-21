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
 * MDC Events
 *
 * @{
 */

#define MDC_EVENT_BASE          1    /**< Max 1000 basis events. */

/** MDC Events */
enum MDC_event
{
    MDC_red_pressed = MDC_EVENT_BASE,
    MDC_red_released,
    MDC_green_pressed,
    MDC_green_released,
    MDC_blue_pressed,
    MDC_blue_released,
    MDC_yellow_pressed,
    MDC_yellow_released
};

extern USO_list_t MDC_event_slots;
extern USO_buf_pool_t MDC_event_pool;

/**
 */
extern void MDC_events_init (void);

/**
 */
extern void MDC_event_create (enum MDC_event event);

/**
 */
extern void MDC_events_create (void);



/** @}
 */

/** @}
 */

#endif /* EVENTS_H_ */
