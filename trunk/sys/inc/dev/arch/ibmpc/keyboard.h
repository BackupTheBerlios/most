
#ifndef IBMPC_KEYBOARD_H
#define IBMPC_KEYBOARD_H

#include <uso/pipe.h>
#include <uso/barrier.h>
#include <uso/thread.h>
#include <dev/arch/ibmpc/types.h>

#define IBMPC_KBD_KEYCODE_BUF_SIZE (2*64)
#define IBMPC_KBD_DATA_BUF_SIZE (32)

/*
 * Keyboard Struct
 *
 * Private.
 */
struct IBMPC_keyboard
{
    char keycode_buffer[IBMPC_KBD_KEYCODE_BUF_SIZE];
    USO_pipe_t keycode_pipe;
    USO_barrier_t keycode_avail;
    char data_buffer[IBMPC_KBD_DATA_BUF_SIZE];
    USO_pipe_t data_pipe;
    USO_barrier_t data_avail;
    USO_thread_t *thread;
};

/** Keyboard device type. */
typedef struct IBMPC_keyboard IBMPC_keyboard_t;

extern void IBMPC_keyboard_init (IBMPC_keyboard_t *kbd);

void IBMPC_keyboard_install(IBMPC_keyboard_t *kbd, char *name);

extern void IBMPC_keyboard_start (IBMPC_keyboard_t *kbd, int keyboard_stack_size);

/**
 * IRQ-Hander
 */
void IBMPC_keyboard_irq_handler(IBMPC_keyboard_t *kbd);


#endif
