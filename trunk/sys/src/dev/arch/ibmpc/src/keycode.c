/*
 * Copyright (c) 2008 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Antoine Kaufmann.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *     This product includes software developed by the tyndur Project
 *     and its contributors.
 * 4. Neither the name of the tyndur Project nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>

#include <dev/arch/ibmpc/vt100_input.h>
#include <dev/arch/ibmpc/keymap.h>
#include <dev/arch/ibmpc/keycode.h>


struct modifiers {
	ACE_bool_t shift;
	ACE_bool_t control;
	ACE_bool_t alt;
	ACE_bool_t altgr;
};

static struct modifiers modifiers = {
		.shift = FALSE,
		.control = FALSE,
		.alt = FALSE,
		.altgr = FALSE,
};


extern int IBMPC_translate_keycode(ACE_u8_t keycode, ACE_bool_t down, char *buf, ACE_size_t buf_size)
{
    keymap_entry_t* e;
    int len = 0;
    int vt100_len;
    char c = 0;

    // Modifier-Tasten Verarbeiten
    switch (keycode) {
        case KEYCODE_SHIFT_LEFT:
        case KEYCODE_SHIFT_RIGHT:
            modifiers.shift = down;
            return 0;

        case KEYCODE_CONTROL_LEFT:
        case KEYCODE_CONTROL_RIGHT:
            modifiers.control = down;
            return 0;

        case KEYCODE_ALT:
            modifiers.alt = down;
            return 0;

        case KEYCODE_ALTGR:
            modifiers.altgr = down;
            return 0;
    }

    // Von hier an brauchen wir die losgelassenen Tasten nicht mehr
    if (!down) {
        return 0;
    }

    // Wenn alt gedrueckt wurde, senden wir vorher einfach ein ESC
    if (modifiers.alt && buf_size > 0) {
        *buf++ = '\033';
        len++;
        buf_size--;
    }

    // Eingabe an vt100-Emulation weiterleiten
    vt100_len = IBMPC_vt100_translate_keycode(keycode, buf, buf_size);
    if (vt100_len != -1) {
        return len + vt100_len;
    }

    // Zeichen auswaehlen
    e = IBMPC_keymap_get(keycode);
    if (modifiers.shift) {
        c = e->shift;
    } else if (modifiers.altgr) {
        c = e->altgr;
    } else if (modifiers.control) {
        c = e->ctrl;
    } else {
        c = e->normal;
    }

    if ( (c != 0) && buf_size > 0) {
        *buf++ = c;
        len++;
    }
    return len;
}

