/*
 * kbd.h
 *
 *  Created on: 31.05.2013
 *      Author: maik
 */

#ifndef PC_KBD_H_
#define PC_KBD_H_

#include <dev/arch/ibmpc/console.h>

extern void PC_kbd_interrupt(void);
extern void PC_kbd_install(IBMPC_console_t *con);


#endif /* KBD_H_ */
