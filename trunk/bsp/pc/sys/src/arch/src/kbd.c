/*
 * kbd.c
 *
 *  Created on: 31.05.2013
 *      Author: maik
 */

#include <dev/arch/ibmpc/keyboard.h>
#include <arch/kbd.h>

static IBMPC_keyboard_t kbd0;

extern void PC_kbd_interrupt(void)
{
	IBMPC_keyboard_irq_handler(&kbd0);
}

extern void PC_kbd_install(void)
{
	IBMPC_keyboard_install(&kbd0, "kbd0");
}

extern void PC_keyboard_start (void)
{
	IBMPC_keyboard_start (&kbd0, 0x2000);
}

