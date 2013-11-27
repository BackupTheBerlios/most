/*
 * vt100_input.c
 *
 *  Created on: 02.06.2013
 *      Author: maik
 */

#ifndef VT100_INPUT_H_
#define VT100_INPUT_H_

#include <ace/stddef.h>

/**
 * Eingabe fuer vt100-Emulation verarbeiten
 *
 * @param vterm     VTerm-Handle
 * @param keycode   Keycode der gedrueckten Taste
 * @param buffer    Puffer in dem das Resultat abgelegt werden kann
 * @param buffer_sz Groesse des Puffers
 *
 * @return Anzahl der Zeichen die neu im Puffer sind oder -1 wenn die Emulation
 *         nichts mit der Taste anfangen kann
 */
extern int IBMPC_vt100_input(ACE_u8_t keycode, char* buffer, ACE_size_t buffer_sz);


#endif /* VT100_INPUT_C_ */
