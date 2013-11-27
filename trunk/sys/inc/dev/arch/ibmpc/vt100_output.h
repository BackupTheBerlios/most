/*
 * vt100_output.h
 *
 *  Created on: 23.11.2013
 *      Author: maik
 */

#ifndef IBMPC_VT100_OUTPUT_H_
#define IBMPC_VT100_OUTPUT_H_

#include <dev/arch/ibmpc/console.h>

enum IBMPC_vt100_output_status
{
    IBMPC_VT100_OUT_INVALID = 0,
    IBMPC_VT100_OUT_SUCCESS,
    IBMPC_VT100_OUT_NEED_MORE
};


/**
 * Versucht eine ANSI-Escape-Code-Sequenz zu parsen.
 *
 * @param ansi_buf Sequenz die ausgewertet werden soll
 * @param ansi_buf_len Anzahl der auszuwertenden Bytes
 *
 * @return INVALID, wenn die ANSI-Code-Sequenz ungueltig ist, SUCCESS, wenn die
 *    ANSI-Code-Sequenz erfolgreich ausgewertet wurde, NEED_MORE, wenn die
 *    Sequenz unvollstaendig ist, und noch weitere Zeichen zur Auswertung
 *    eingelesen werden muessen.
 */
extern enum IBMPC_vt100_output_status IBMPC_vt100_output(IBMPC_console_t *con, const char *buf, unsigned int buf_len);

#endif /* VT100_OUTPUT_H_ */
