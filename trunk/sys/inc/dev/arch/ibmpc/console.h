#ifndef IBMPC_CONSOLE_H
#define IBMPC_CONSOLE_H

#include <uso/pipe.h>
#include <uso/barrier.h>
#include <dev/arch/ibmpc/screen.h>
#include <dev/arch/ibmpc/keycode.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup console console.h
 *
 * Console
 * @{
 */

#define IBMPC_CONSOLE_READ_BUF_SIZE (128)
#define IBMPC_CONSOLE_TERM_MSG_SIZE   32


/*
 * Console Struct
 *
 * Private.
 */
struct IBMPC_console
{
    IBMPC_keycode_modifiers_t modifiers;
    IBMPC_screen_t screen;
    char read_buffer[IBMPC_CONSOLE_READ_BUF_SIZE];
    USO_pipe_t read_pipe;
    USO_barrier_t read_data_avail;
    char ansi_buf[16];                   // Puffer fuer die ANSI-Sequenzen
    unsigned int ansi_buf_offs;
    unsigned int save_cursor_x;
    unsigned int save_cursor_y;
};

/** Console device type. */
typedef struct IBMPC_console IBMPC_console_t;


extern void IBMPC_console_install (IBMPC_console_t *con, char *name);

extern void IBMPC_console_process_key (IBMPC_console_t *con, ACE_u8_t keycode, ACE_bool_t break_code);

extern void IBMPC_console_process_term (IBMPC_console_t *con, char *string);

#endif /* CONSOLE_H */
