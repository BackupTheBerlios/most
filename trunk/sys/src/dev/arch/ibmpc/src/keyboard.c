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
#include <uso/thread.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>
#include <dev/arch/ibmpc/ports.h>
#include <dev/arch/ibmpc/types.h>
#include <dev/arch/ibmpc/keyboard.h>
#include <dev/arch/ibmpc/keycode.h>
#include <dev/arch/ibmpc/scancode.h>




/**
 * Wird Benutzt um Meldungen ueber ungueltige Scancodes waehrend der
 * Initialisierungsphase zu verhindern
 */
static ACE_bool_t init_done = FALSE;


/**
 * Befehl an die Tastatur senden
 */
static void send_kbd_command(ACE_u8_t command)
{
    while ((inb(0x64) & 0x6) != 0x4) USO_yield();
    outb(0x60, command);
}


/**
 * Tastaturtreiber initialisieren
 */
static void keyboard_init(void)
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
    init_done = TRUE;
}



/**
 * IRQ-Hander
 */
void IBMPC_keyboard_irq_handler(IBMPC_keyboard_t *kbd) {
    // Status-Variablen fuer das Behandeln von e0- und e1-Scancodes
    static ACE_bool_t     e0_code = FALSE;
    // Wird auf 1 gesetzt, sobald e1 gelesen wurde, und auf 2, sobald das erste
    // Datenbyte gelesen wurde
    static int e1_code = 0;
    static ACE_u16_t  e1_prev = 0;

    ACE_u8_t keycode = 0;
    ACE_u8_t scancode;
    ACE_bool_t break_code = FALSE;
	ACE_u16_t buf = 0;


    scancode = inb(0x60);

    // Abbrechen wenn die Initialisierung noch nicht abgeschlossen wurde
    if (!init_done) {
        return;
    }


    // Um einen Breakcode handelt es sich, wenn das oberste Bit gesetzt ist und
    // es kein e0 oder e1 fuer einen Extended-scancode ist
    if ((scancode & 0x80) &&
        (e1_code || (scancode != 0xE1)) &&
        (e0_code || (scancode != 0xE0)))
    {
        break_code = TRUE;
        scancode &= ~0x80;
    }

    if (e0_code) {
        // Fake shift abfangen
        // Siehe: http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html#ss1.6
        if ((scancode == 0x2A) || (scancode == 0x36)) {
            e0_code = FALSE;
            return;
        }

        // Fertiger e0-Scancode
        keycode = IBMPC_translate_scancode(1, scancode);
        e0_code = FALSE;
    } else if (e1_code == 2) {
        // Fertiger e1-Scancode
        // Zweiten Scancode in hoeherwertiges Byte packen
        e1_prev |= ((ACE_u16_t) scancode << 8);
        keycode = IBMPC_translate_scancode(2, e1_prev);
        e1_code = 0;
    } else if (e1_code == 1) {
        // Erstes Byte fuer e1-Scancode
        e1_prev = scancode;
        e1_code++;
    } else if (scancode == 0xE0) {
        // Anfang eines e0-Codes
        e0_code = TRUE;
    } else if (scancode == 0xE1) {
        // Anfang eines e1-Codes
        e1_code = 1;
    } else {
        // Normaler Scancode
        keycode = IBMPC_translate_scancode(0, scancode);
    }

    if (keycode != 0) {
    	buf |= keycode;
    	if (break_code == TRUE){
    		buf |= (1<<8);
    	}
        //USO_log_printf(USO_LL_INFO, " B:%04X ", buf);
    	if (USO_pipe_write_ns (&kbd->keycode_pipe, (char *)&buf, sizeof (buf)) < sizeof (buf))
    	{
    		// todo kbd->error.keycode_buf_overrun++;
    	}
    	USO_go (&kbd->keycode_avail);
    }
}

static ACE_err_t keyboard_open(MFS_descriptor_t * desc)
{
    //IBMPC_keyboard_t *kbd = (IBMPC_keyboard_t *) desc->represent;
    return ACE_OK;
}

static struct MFS_descriptor_op keyboard_desc_op = {
	.open = keyboard_open,
    .close = NULL,
    .info = NULL,
    .control = NULL
};

static ACE_size_t
keyboard_read (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    IBMPC_keyboard_t *kbd = (IBMPC_keyboard_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret;
    ret = 0;
    while (ret == 0){
        USO_cpu_status_t ps = USO_disable ();
    	ret = USO_pipe_read_ns (&kbd->data_pipe, buf, len);
    	if (ret == 0){
    		USO_block (&kbd->data_avail);
    	}
        USO_restore (ps);
    }
    stream->pos_rx += ret;
    return ret;
}

static struct MFS_stream_op keyboard_stream_op = {
    .read = keyboard_read,
    .write =NULL,
    .seek = NULL,
    .flush = NULL
};


extern void
IBMPC_keyboard_init (IBMPC_keyboard_t *kbd)
{
	USO_pipe_init (&kbd->keycode_pipe, kbd->keycode_buffer, sizeof (kbd->keycode_buffer));
	USO_barrier_init (&kbd->keycode_avail);
	USO_pipe_init (&kbd->data_pipe, kbd->data_buffer, sizeof (kbd->data_buffer));
	USO_barrier_init (&kbd->data_avail);
	kbd->thread = NULL;
}

extern void
IBMPC_keyboard_install (IBMPC_keyboard_t *kbd, char *name)
{
	IBMPC_keyboard_init (kbd);
	MFS_stream_create (MFS_resolve(MFS_get_root(), "sys/dev/serial"), name, &keyboard_desc_op,
                   &keyboard_stream_op, (MFS_represent_t *) kbd, MFS_STREAM_IO);
}



static void
keyboard_run (void *param)
{
	IBMPC_keyboard_t *kbd = (IBMPC_keyboard_t*) param;
    USO_log_puts (USO_LL_INFO, "Keyboard is running.\n");
    for (;;)
    {
    	USO_cpu_status_t ps;
        static ACE_u16_t keycode;
        ACE_bool_t break_code;
        static char char_buf[32];
    	ACE_size_t len = 0;
        while (len == 0){
            ps = USO_disable ();
        	len = USO_pipe_read_ns (&kbd->keycode_pipe,(char *) &keycode, sizeof(keycode));
        	if (len == 1){ ACE_PANIC ("keybuf out of sync\n"); }
        	if (len == 0){
        		USO_block (&kbd->keycode_avail);
        	}
            USO_restore (ps);
        }
        break_code = ( (keycode & 0x0100) == 0x0100) ? TRUE : FALSE;
        //USO_log_printf(USO_LL_INFO, " K:%04X ", keycode);
        len = IBMPC_translate_keycode( (ACE_u8_t)keycode, !break_code, char_buf, sizeof (char_buf) );
        ps = USO_disable ();
        if (USO_pipe_write_ns (&kbd->data_pipe, char_buf, len) < len)
        {
        	// todo kbd->error.data_buf_overrun++;
        }
    	USO_go (&kbd->data_avail);
        USO_restore (ps);
    }
}


extern void
IBMPC_keyboard_start (IBMPC_keyboard_t *kbd, int keyboard_stack_size)
{
    kbd->thread = USO_thread_new (keyboard_run,
                                    keyboard_stack_size, USO_INTERRUPT, USO_FIFO, "keybd");
    if (kbd->thread != NULL){
        USO_thread_arg_init (kbd->thread, kbd);
    	USO_start (kbd->thread);
    	keyboard_init();
    }
}
