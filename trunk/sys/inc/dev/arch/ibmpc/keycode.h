/*
 * keycode.h
 *
 *  Created on: 02.06.2013
 *      Author: maik
 */

#ifndef KEYCODE_H_
#define KEYCODE_H_

#include <ace/stddef.h>

struct IBMPC_keycode_modifiers {
    ACE_bool_t shift;
    ACE_bool_t control;
    ACE_bool_t alt;
    ACE_bool_t altgr;
};

typedef struct IBMPC_keycode_modifiers IBMPC_keycode_modifiers_t;

/**
 * Es handelt sich dabei aber nur um diese Keycodes, die auf allen Layouts am
 * selben Ort sind.
 */

#define KEYCODE_SHIFT_LEFT      42
#define KEYCODE_SHIFT_RIGHT     54
#define KEYCODE_CONTROL_LEFT    29
#define KEYCODE_CONTROL_RIGHT   97
#define KEYCODE_ALT             56
#define KEYCODE_ALTGR           100

#define KEYCODE_INSERT          110
#define KEYCODE_DELETE          111
#define KEYCODE_PAGE_UP         104
#define KEYCODE_PAGE_DOWN       109
#define KEYCODE_HOME            102
#define KEYCODE_END             107

#define KEYCODE_F1              59
#define KEYCODE_F2              60
#define KEYCODE_F3              61
#define KEYCODE_F4              62
#define KEYCODE_F5              63
#define KEYCODE_F6              64
#define KEYCODE_F7              65
#define KEYCODE_F8              66
#define KEYCODE_F9              67
#define KEYCODE_F10             68
#define KEYCODE_F11             87
#define KEYCODE_F12             88

#define KEYCODE_ARROW_UP        103
#define KEYCODE_ARROW_DOWN      108
#define KEYCODE_ARROW_LEFT      105
#define KEYCODE_ARROW_RIGHT     106

#define KEYCODE_SCROLL_LOCK     70

/**
 * @param keycode   Der Keycode
 * @param down      TRUE wenn die Taste gedrueckt wurde, FALSE sonst
 */
extern int IBMPC_translate_keycode(IBMPC_keycode_modifiers_t *modifiers, ACE_u8_t keycode, ACE_bool_t down,
                                   char *buf, ACE_size_t buf_size);

extern void IBMPC_keycode_init(IBMPC_keycode_modifiers_t *modifiers);


#endif /* KEYCODE_H_ */
