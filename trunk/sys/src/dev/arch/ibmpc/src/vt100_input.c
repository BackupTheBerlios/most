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

#include <ace/string.h>
#include <dev/arch/ibmpc/keycode.h>

#include <dev/arch/ibmpc/vt100_input.h>


extern int
IBMPC_vt100_input(ACE_u8_t keycode, char* buffer, ACE_size_t buffer_sz)
{
    const char* res = NULL;

    switch (keycode) {
        case KEYCODE_ARROW_UP:
            res = "\033[A";
            break;

        case KEYCODE_ARROW_DOWN:
            res = "\033[B";
            break;

        case KEYCODE_ARROW_RIGHT:
            res = "\033[C";
            break;

        case KEYCODE_ARROW_LEFT:
            res = "\033[D";
            break;

        case KEYCODE_HOME:
            res = "\033[1~";
            break;

        case KEYCODE_END:
            res = "\033OF";
            break;

        case KEYCODE_PAGE_UP:
            res = "\033[5~";
            break;

        case KEYCODE_PAGE_DOWN:
            res = "\033[6~";
            break;

        case KEYCODE_INSERT:
            res = "\033[2~";
            break;

        case KEYCODE_DELETE:
            res = "\033[3~";
            break;

        case KEYCODE_F1:
            res = "\033OP";
            break;

        case KEYCODE_F2:
            res = "\033OQ";
            break;

        case KEYCODE_F3:
            res = "\033OR";
            break;

        case KEYCODE_F4:
            res = "\033OS";
            break;

        case KEYCODE_F5:
            res = "\033[15~";
            break;

        case KEYCODE_F6:
            res = "\033[17~";
            break;

        case KEYCODE_F7:
            res = "\033[18~";
            break;

        case KEYCODE_F8:
            res = "\033[19~";
            break;

        case KEYCODE_F9:
            res = "\033[20~";
            break;

        case KEYCODE_F10:
            res = "\033[21~";
            break;

        case KEYCODE_F11:
            res = "\033[23~";
            break;

        case KEYCODE_F12:
            res = "\033[24~";
            break;
    }

    if (res) {
        ACE_strncpy(buffer, res, buffer_sz);
        return ACE_strlen(res);
    }

    return -1;
}

