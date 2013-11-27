/*
 * scancode.h
 *
 *  Created on: 02.06.2013
 *      Author: maik
 */

#ifndef SCANCODE_H_
#define SCANCODE_H_

#include <ace/stddef.h>

/**
 * Scancode in einen Keycode uebersetzen
 *
 * @param set Zu Benutztende Tabelle:
 *              0: Normale Scancodes
 *              1: Extended0 Scancodes
 *              2: Extended1 Scancodes
 *
 * @param code Scancode; keine Breakcodes nur normale Scancodes
 *             Fuer e1 den zweiten Scancode im hoeherwertigen Byte uebergeben
 *
 * @return Keycode oder 0 falls der Scancode nicht bekannt ist
 */
ACE_u8_t IBMPC_translate_scancode(int set, ACE_u16_t scancode);


#endif /* SCANCODE_H_ */
