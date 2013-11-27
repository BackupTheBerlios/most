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

extern void PC_kbd_install(IBMPC_console_t *con)
{
    IBMPC_keyboard_init(&kbd0, con);
    IBMPC_keyboard_open (&kbd0);
}

