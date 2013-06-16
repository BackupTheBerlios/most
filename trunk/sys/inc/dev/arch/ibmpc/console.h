#ifndef IBMPC_CONSOLE_H
#define IBMPC_CONSOLE_H

/** @addtogroup dev
 * @{
 */

/** @defgroup console console.h
 *
 * Console
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Console Struct
 *
 * Private.
 */
struct IBMPC_console
{
};

/** Console device type. */
typedef struct IBMPC_console IBMPC_console_t;


extern void IBMPC_console_install (IBMPC_console_t *con, char *name);


#endif /* ndef CONSOLE_H */
