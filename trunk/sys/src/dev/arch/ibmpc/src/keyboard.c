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

#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <dev/arch/ibmpc/ports.h>
#include <dev/arch/ibmpc/keyboard.h>
#include <dev/arch/ibmpc/scancode.h>


/**
 * Befehl an die Tastatur senden
 */
static void send_kbd_command(ACE_u8_t command)
{
    while ((inb(0x64) & 0x6) != 0x4) USO_yield();
    outb(0x60, command);
}


/**
 * IRQ-Hander
 */
void IBMPC_keyboard_irq_handler(IBMPC_keyboard_t *kbd) {
    ACE_bool_t break_code = FALSE;
    ACE_u8_t keycode = 0;
    ACE_u8_t scancode = inb(0x60);

    // Abbrechen wenn die Initialisierung noch nicht abgeschlossen wurde
    if (kbd->init_done == FALSE) {
        return;
    }

    // Um einen Breakcode handelt es sich, wenn das oberste Bit gesetzt ist und
    // es kein e0 oder e1 fuer einen Extended-scancode ist
    if ((scancode & 0x80) &&
        (kbd->e1_code || (scancode != 0xE1)) &&
        (kbd->e0_code || (scancode != 0xE0)))
    {
        break_code = TRUE;
        scancode &= ~0x80;
    }

    if (kbd->e0_code) {
        // Fake shift abfangen
        // Siehe: http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html#ss1.6
        if ((scancode == 0x2A) || (scancode == 0x36)) {
            kbd->e0_code = FALSE;
            return;
        }

        // Fertiger e0-Scancode
        keycode = IBMPC_translate_scancode(1, scancode);
        kbd->e0_code = FALSE;
    } else if (kbd->e1_code == 2) {
        // Fertiger e1-Scancode
        // Zweiten Scancode in hoeherwertiges Byte packen
        kbd->e1_prev |= ((ACE_u16_t) scancode << 8);
        keycode = IBMPC_translate_scancode(2, kbd->e1_prev);
        kbd->e1_code = 0;
    } else if (kbd->e1_code == 1) {
        // Erstes Byte fuer e1-Scancode
        kbd->e1_prev = scancode;
        kbd->e1_code++;
    } else if (scancode == 0xE0) {
        // Anfang eines e0-Codes
        kbd->e0_code = TRUE;
    } else if (scancode == 0xE1) {
        // Anfang eines e1-Codes
        kbd->e1_code = 1;
    } else {
        // Normaler Scancode
        keycode = IBMPC_translate_scancode(0, scancode);
    }

    if (keycode != 0) {
        IBMPC_console_process_key (kbd->con, keycode, break_code);
    }
}

extern void
IBMPC_keyboard_open (IBMPC_keyboard_t *kbd)
{
    // So, mal hoeren was uns die Tastatur noch so alles zu erzaehlen hat von
    // eventuell gedrueckten Tasten waerend dem Booten.
    while ((inb(0x64) & 0x1)) {
        inb(0x60);
    }

    // Leds alle ausloeschen
    send_kbd_command(0xED);
    outb(0x60, 0);

    // Schnellste Wiederholrate
    send_kbd_command(0xF3);
    outb(0x60, 0);

    // Tastatur aktivieren
    send_kbd_command(0xF4);

    kbd->init_done = TRUE;
}

extern void
IBMPC_keyboard_init (IBMPC_keyboard_t *kbd, IBMPC_console_t *con)
{
    kbd->con = con;
    kbd->init_done = FALSE;
    kbd->e0_code = FALSE;
    kbd->e1_code = 0;
    kbd->e1_prev = 0;
}

