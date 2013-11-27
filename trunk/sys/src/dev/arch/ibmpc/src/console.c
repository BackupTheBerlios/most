/*
 * Copyright (c) 2006 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Burkhard Weseloh.
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
#include <ace/stdio.h>
#include <ace/ascii.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <dev/arch/ibmpc/vt100_output.h>
#include <dev/arch/ibmpc/console.h>
#include <dev/err.h>



/**
 * Gibt ein Zeichen auf der Konsole aus. Wenn das Zeichen ein Escape-Zeichen
 * ist, wird es und die folgenden Zeichen in einem Puffer zwischen gespeichert,
 * bis entweder diese Zeichenfolge erfolgreich als ANSI-Escape-Code-Sequenz
 * ausgewertet wurde, oder feststeht, dass diese Sequenz ungueltig ist. In
 * diesem Fall werden alle Zeichen einschliesslich des Escape-Zeichens auf der
 * Konsole ausgegeben.
 */
static void con_putc_ansi(IBMPC_console_t *con, const char c)
{
    enum IBMPC_vt100_output_status status;
    int i;

    if(c == ACE_ASCII_ESC || con->ansi_buf_offs > 0)
    {
        con->ansi_buf[con->ansi_buf_offs++] = c;

        status = IBMPC_vt100_output(con, con->ansi_buf, con->ansi_buf_offs);

        switch(status)
        {
            case IBMPC_VT100_OUT_SUCCESS:
                con->ansi_buf_offs = 0;
                break;
            case IBMPC_VT100_OUT_NEED_MORE:
                if(con->ansi_buf_offs <= sizeof(con->ansi_buf))
                {
                    break;
                }
                /* no break */
            case IBMPC_VT100_OUT_INVALID:
            default:
                for(i = 0; i < con->ansi_buf_offs; i++)
                {
                    IBMPC_screen_putc(&con->screen, con->ansi_buf[i]);
                }
                con->ansi_buf_offs = 0;
                break;
        }
    }
    else
    {
        IBMPC_screen_putc(&con->screen, c);
    }
}


static ACE_err_t console_open(MFS_descriptor_t * desc)
{
    if (desc->open_cnt == 0){
    }
    return ACE_OK;
}

static struct MFS_descriptor_op console_desc_op = {
                .open = console_open,
                .close = NULL,
                .info = NULL,
                .control = NULL,
                .delete = NULL
};

static ACE_size_t
console_read (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    IBMPC_console_t *con = (IBMPC_console_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret;
    ret = 0;
    while (ret == 0){
        USO_cpu_status_t ps = USO_disable ();
        ret = USO_pipe_read_ns (&con->read_pipe, buf, len);
        if (ret == 0){
            USO_block (&con->read_data_avail);
        }
        USO_restore (ps);
    }
    stream->pos_rx += ret;
    return ret;
}

/**
 * Gibt eine Zeichenkette auf der Konsole aus. Diese Zeichenkette kann
 * ANSI-Steuercodes enthalten. Die Ausgabe terminiert bei Erreichen eines
 * Null-Bytes oder wenn n Bytes ausgewertet wurden.
 * Unvollstaendige und fehlerhafte Sequenzen werden einfach ausgegeben.
 */
static ACE_size_t
console_write(MFS_stream_t *stream, const char *s, ACE_size_t n)
{
    IBMPC_console_t *con = (IBMPC_console_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret;
    ret = n;
    while(*s && n--)
    {
        con_putc_ansi(con, *s);
        s++;
    }

    ret -= n;
    stream->size_tx += ret;
    return ret;
}



static struct MFS_stream_op console_stream_op = {
                .read = console_read,
                .write = console_write,
                .seek = NULL,
                .flush = NULL
};

/**
 * Initialisiert die Console, in dem der Bildschirm geleert wird.
 */
static void
console_init (IBMPC_console_t *con)
{
    IBMPC_keycode_init(&con->modifiers);
    IBMPC_screen_init(&con->screen);
    USO_pipe_init (&con->read_pipe, con->read_buffer, sizeof (con->read_buffer));
    USO_barrier_init (&con->read_data_avail);
    con->ansi_buf_offs = 0;
}

extern void
IBMPC_console_install (IBMPC_console_t *con, char *name)
{
    console_init(con);
    MFS_descriptor_t * dir = MFS_resolve("/sys/dev/serial");
    MFS_stream_create (dir, name, &console_desc_op,
                   &console_stream_op, (MFS_represent_t *) con, MFS_STREAM_IO);
    MFS_close_desc(dir);
}

extern void
IBMPC_console_process_key (IBMPC_console_t *con, ACE_u8_t keycode, ACE_bool_t break_code)
{
    /* we are running on interrupt context! */
    static char buf[32];
    ACE_size_t len = 0;
    len = IBMPC_translate_keycode(&con->modifiers, (ACE_u8_t)keycode, !break_code, buf, sizeof (buf) );
    if (USO_pipe_write_ns (&con->read_pipe, buf, len) < len)
    {
        // todo con->error.read_buf_overrun++;
    }
    USO_go(&con->read_data_avail);
}

extern void
IBMPC_console_process_term (IBMPC_console_t *con, char *string)
{
    int len = ACE_strnlen(string, IBMPC_CONSOLE_TERM_MSG_SIZE);
    USO_cpu_status_t ps = USO_disable ();
    if (USO_pipe_write_ns (&con->read_pipe, string, len) < len)
    {
        // todo con->error.read_buf_overrun++;
    }
    USO_restore (ps);
    USO_go(&con->read_data_avail);
}
