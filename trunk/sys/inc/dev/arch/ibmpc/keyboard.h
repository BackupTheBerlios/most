
#ifndef IBMPC_KEYBOARD_H
#define IBMPC_KEYBOARD_H

#include <ace/string.h>
#include <ace/stdio.h>
#include <dev/arch/ibmpc/console.h>
#include <dev/err.h>

/*
 * Keyboard Struct
 *
 * Private.
 */
struct IBMPC_keyboard
{
    IBMPC_console_t *con;
    ACE_bool_t init_done;
    ACE_bool_t e0_code;   // Status-Variablen fuer das Behandeln von e0- und e1-Scancodes
    int e1_code;          // Wird auf 1 gesetzt, sobald e1 gelesen wurde, und auf 2, sobald das erste Datenbyte gelesen wurde
    ACE_u16_t  e1_prev;
};

/** Keyboard device type. */
typedef struct IBMPC_keyboard IBMPC_keyboard_t;

/**
 * IRQ-Hander
 */
void IBMPC_keyboard_irq_handler(IBMPC_keyboard_t *kbd);

extern void IBMPC_keyboard_open (IBMPC_keyboard_t *kbd);

extern void IBMPC_keyboard_init (IBMPC_keyboard_t *kbd, IBMPC_console_t *con);



#endif
