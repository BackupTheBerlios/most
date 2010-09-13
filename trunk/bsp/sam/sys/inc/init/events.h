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

#define SAM_EVENT_BASE          1    /** Max 1000 basis events. */
#define SAM_LCD_EVENT_BASE   1000    /** Max 1000 lcd events. */

enum SAM_event {
	SAM_switch_1_pressed = SAM_EVENT_BASE,
	SAM_switch_2_pressed,
	SAM_joystick_up_pressed,
	SAM_joystick_down_pressed,
	SAM_joystick_left_pressed,
	SAM_joystick_right_pressed,
	SAM_joystick_center_pressed,
	SAM_switch_1_not_pressed,
	SAM_switch_2_not_pressed,
	SAM_joystick_up_not_pressed,
	SAM_joystick_down_not_pressed,
	SAM_joystick_left_not_pressed,
	SAM_joystick_right_not_pressed,
	SAM_joystick_center_not_pressed,
	SAM_card_inserted,
	SAM_card_removed
};

extern USO_list_t SAM_event_slots;
extern USO_buf_pool_t SAM_event_pool;

extern void SAM_events_init(void);

extern void SAM_event_create(enum SAM_event event);

extern void SAM_events_create(void);

#endif /* EVENTS_H_ */
