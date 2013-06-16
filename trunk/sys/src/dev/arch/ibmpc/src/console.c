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
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <dev/arch/ibmpc/console.h>
#include <dev/arch/ibmpc/types.h>
#include <dev/arch/ibmpc/ports.h>
#include <dev/err.h>

/*
 * Das hier ist eine sehr beschr�nkte Untermenge der ANSI Escape Codes. Alle
 * Sequenzen werden mit dem ASCII Zeichen ESC (Wert 0x1b) eingeleitet. ESC
 * soll im folgenden dieses Zeichen darstellen. Wir k�nnen folgende Escape 
 * Sequenzen behandeln:
 *
 * ESC[#A - Cursor um # Zeilen nach oben bewegen
 * ESC[#B - Cursor um # Zeilen nach unten bewegen
 * ESC[#C - Cursor um # Zeilen nach rechts bewegen
 * ESC[#D - Cursor um # Zeilen nach links bewegen
 * ESC[2J - Bildschirm leeren
 * ESC[K - Bis Zeile von der Cursor-Position zum Ende leeren
 * ESC[#m oder ESC[#;#m - Vorder- und Hintergrundfarbe �ndern, Fettschrift, Blinken
 * ESC[s - Cursor-Position speichern
 * ESC[u - Cursor-Position wiederherstellen
 *
 * Folgende Farbcodes werden bei und ESC[#m oder ESC[#;#m unterst�tzt:
 * - Attribute:
 *  0 - Normale Schrift
 *  1 - Fett
 *  5 - Blinken
 * - Vordergrundfarben:
 *  30 - Schwarz
 *  31 - Rot
 *  32 - Gr�n
 *  33 - Gelb
 *  34 - Blau
 *  35 - Magenta
 *  36 - Cyan
 *  37 - Weiss
 * - Hintergrundfarben:
 *  40 - Schwarz
 *  41 - Rot
 *  42 - Gr�n
 *  43 - Gelb
 *  44 - Blau
 *  45 - Magenta
 *  46 - Cyan
 *  47 - Weiss
 *
 * Es k�nnen nur 1 oder 2 Farbcodes pro Escape-Sequenz angewendet werden.
 *
 * Ung�ltige und nicht erkannte Sequenzen werden einfach ausgegeben.
 */

#define ASCII_ESC 0x1b

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25


static unsigned short * vidmem = (unsigned short*)0xb8000;

// Aktuelle Cursor-Position
static unsigned int cursor_x;
static unsigned int cursor_y;

typedef enum ansi_esc_seq_status
{
    INVALID = 0,
    SUCCESS,
    NEED_MORE,
} ansi_esc_seq_status_t;

// Puffer f�r die ANSI-Sequenzen
static char ansi_buf[16];
static unsigned int ansi_buf_ofs = 0;

// Aufschl�sselung des Attribut-Bytes vom Text Modus
static union
{
    unsigned char _color;
    struct 
    {
        unsigned char foreground : 3;
        unsigned char bold : 1;
        unsigned char background : 3;
        unsigned char blink : 1;
    }_attr;
} con_color;


/**
 * Setzt die Cursor-Position
 */
static void con_set_cursor_pos(unsigned int x, unsigned int y)
{
    if(x >= SCREEN_WIDTH)
    {
        cursor_x = SCREEN_WIDTH - 1;
    }
    else
    {
        cursor_x = x;
    }

    if(y >= SCREEN_HEIGHT)
    {
        cursor_y = SCREEN_HEIGHT - 1;
    }
    else
    {
        cursor_y = y;
    }

}

/**
 * Leert den Bildschirm
 */
static void con_clear_screen(void)
{
    int i;

    for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        vidmem[i] = con_color._color << 8;
    }
    
    con_set_cursor_pos(0, 0);
}

/**
 * Scrollt eine Zeile nach unten
 */
static void scroll_down(void)
{
    int i;
    
    memmove(vidmem, vidmem + SCREEN_WIDTH, SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2);

    for(i = SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        vidmem[i] = con_color._color << 8;
    }

    con_set_cursor_pos(cursor_x, cursor_y - 1);
}

/**
 * Setzt die Attribute f�r die folgenden Ausgabe entsprechend der obigen Liste.
 */
static ansi_esc_seq_status_t handle_ansi_formatting_sequence_number(int n)
{
    static char colors[8] = { 0, 4, 2, 6, 1, 5, 3, 7 }; /* Zum Umrechnen der ANSI Farbcodes in die VGA Palette. */

    switch(n)
    {
        case 0: // Normal
            con_color._attr.bold = 0;
            con_color._attr.blink = 0;
            break;
        
        case 1: // Fett
            con_color._attr.bold = 1;
            break;

        case 5: // Blinken
            con_color._attr.blink = 1;
            break;

        case 30 ... 37: // Vordergrundfarbe
            con_color._attr.foreground = colors[n - 30];
            break;

        case 40 ... 47: // Hintergrundfarbe
            con_color._attr.background = colors[n - 40];
            break;

        default:
            return INVALID;
    }

    return SUCCESS;
}

/**
 * L�scht den Bildschirm von der aktuellen Cursor Position bis zur
 * angegebenen Spalte
 *
 * @return Die Spalte bis zu der gel�scht wurde
 */
static unsigned int con_clear_to(unsigned int column)
{
    int i;

    for(i = cursor_x; i < column; i++)
    {
        vidmem[cursor_y * SCREEN_WIDTH + i] = con_color._color << 8;
    }

    return column;
}

/**
 * Versucht eine ANSI-Escape-Code-Sequenz zu parsen. 
 *
 * @param ansi_buf Sequenz die ausgewertet werden soll
 * @param ansi_buf_len Anzahl der auszuwertenden Bytes
 * 
 * @return INVALID, wenn die ANSI-Code-Sequenz ung�ltig ist, SUCCESS, wenn die
 *    ANSI-Code-Sequenz erfolgreich ausgewertet wurde, NEED_MORE, wenn die 
 *    Sequenz unvollst�ndig ist, und noch weitere Zeichen zur Auswertung 
 *    eingelesen werden m�ssen.
 */
static ansi_esc_seq_status_t con_ansi_parse(const char * ansi_buf, unsigned int ansi_buf_len)
{
    static unsigned int save_cursor_x;
    static unsigned int save_cursor_y;

    int i;
    int n1 = 0, n2 = 0;
    int have_n1 = 0, have_n2 = 0, have_delimiter = 0;
    
    if(ansi_buf_len == 0)
    {
        return NEED_MORE;
    }

    if(ansi_buf[0] != ASCII_ESC)
    {
        return INVALID;
    }

    if(ansi_buf_len == 1)
    {
        return NEED_MORE;
    }

    if(ansi_buf[1] != '[')
    {
        return INVALID;
    }

    if(ansi_buf_len == 2)
    {
        return NEED_MORE;
    }

    for(i = 2; i < ansi_buf_len; i++)
    {
        switch(ansi_buf[i])
        {
            case '0' ... '9':
                if(!have_delimiter)
                {
                    n1 = n1 * 10 + (ansi_buf[i] - '0');
                    have_n1 = 1;
                }
                else
                {
                    n2 = n2 * 10 + (ansi_buf[i] - '0');
                    have_n2 = 1;
                }
                break; // n�chstes Zeichen auswerten

            case ';':
                if(have_delimiter)
                {
                    return INVALID;
                }
                have_delimiter = 1;
                break; // n�chstes Zeichen auswerten


            case 'A': // ESC[#A - Cursor um # Zeilen nach oben bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return INVALID;
                }
                con_set_cursor_pos(cursor_x, cursor_y - n1);
                return SUCCESS;

            case 'B': // ESC[#B - Cursor um # Zeilen nach unten bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return INVALID;
                }
                con_set_cursor_pos(cursor_x, cursor_y + n1);
                return SUCCESS;

            case 'C': // ESC[#C - Cursor um # Zeilen nach rechts bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return INVALID;
                }
                con_set_cursor_pos(cursor_x + n1, cursor_y);
                return SUCCESS;
                
            case 'D': // ESC[#D - Cursor um # Zeilen nach links bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return INVALID;
                }
                con_set_cursor_pos(cursor_x - n1, cursor_y);
                return SUCCESS;

            case 'H': // ESC[#;#H - Cursor zur Zeile # und Spalte # bewegen
            case 'f':
                if(!have_n1 || !have_n2)
                {
                    return INVALID;
                }
                
                con_set_cursor_pos(n1, n2);
                return SUCCESS;

            case 'J': // ESC[2J - Bildschirm leeren
                if(have_delimiter || !have_n1 || have_n2 || n1 != 2)
                {
                    return INVALID;
                }
                con_clear_screen();
                return SUCCESS;

            case 'K': // ESC[K - Bis Zeile von der Cursor Position zum Ende leeren
                if(have_delimiter || have_n1 || have_n2)
                {
                    return INVALID;
                }
                
                con_clear_to(SCREEN_WIDTH);

                return SUCCESS;

            case 'm': // ESC[#m oder ESC[#;#m - Vorder- und Hintergrundfarbe �ndern, Fettschrift, Blinken
            {
                // Attribute sichern
                unsigned char attr = con_color._color;

                if(!have_n1)
                {
                    return INVALID;
                }

                if(handle_ansi_formatting_sequence_number(n1) != SUCCESS)
                {
                    return INVALID;
                }

                if(have_n2)
                {
                    if(handle_ansi_formatting_sequence_number(n2) != SUCCESS)
                    {
                        // Bereits uebernommene Aenderungen rueckgaengig machen
                        con_color._color = attr;
                        return INVALID;
                    }
                }

                return SUCCESS;
            }

            case 's': // ESC[s - Cursor-Position speichern
                if(have_delimiter || have_n1 || have_n2)
                {
                    return INVALID;
                }
                save_cursor_x = cursor_x;
                save_cursor_y = cursor_y;

                return SUCCESS;

            case 'u': // ESC[u - Cursor-Position wiederherstellen
                if(have_delimiter || have_n1 || have_n2)
                {
                    return INVALID;
                }
                cursor_x = save_cursor_x;
                cursor_y = save_cursor_y;

                return SUCCESS;

            default:
                return INVALID;
        }
    }

    return NEED_MORE;
}

/**
 * Gibt ein Zeichen auf der Konsole aus.
 */
static void con_putc(const char c)
{
    static char last_char = 0;

    switch(c)
    {
        case '\n':
            if (last_char != '\r') {
                con_clear_to(SCREEN_WIDTH);
            }
            cursor_x = 0;
            cursor_y++;
            break;
        
        case '\r':
            cursor_x = 0;
            break;
        
        case '\t':
            cursor_x = con_clear_to((cursor_x & ~7) + 8);
            break;

        default:
            vidmem[cursor_x + cursor_y * SCREEN_WIDTH] = c | (con_color._color << 8);
            cursor_x++;
            break;
    }
    
    if(cursor_x >= SCREEN_WIDTH)
    {
        cursor_y += cursor_x / SCREEN_WIDTH;
        cursor_x %= SCREEN_WIDTH;
    }
    

    while(cursor_y >= SCREEN_HEIGHT)
    {
        scroll_down();
    }

    last_char = c;
}

/**
 * Gibt ein Zeichen auf der Konsole aus. Wenn das Zeichen ein Escape-Zeichen
 * ist, wird es und die folgenden Zeichen in einem Puffer zwischen gespeichert,
 * bis entweder diese Zeichenfolge erfolgreich als ANSI-Escape-Code-Sequenz
 * ausgewertet wurde, oder feststeht, dass diese Sequenz ung�ltig ist. In 
 * diesem Fall werden alle Zeichen einschlie�lich des Escape-Zeichens auf der 
 * Konsole ausgegeben.
 */
static void con_putc_ansi(const char c)
{
    ansi_esc_seq_status_t status;
    int i;

    if(c == ASCII_ESC || ansi_buf_ofs > 0)
    {
        ansi_buf[ansi_buf_ofs++] = c;

        status = con_ansi_parse(ansi_buf, ansi_buf_ofs);

        switch(status)
        {
            case NEED_MORE:
                if(ansi_buf_ofs <= sizeof(ansi_buf))
                {
                    break;
                }
                // fall through

            case INVALID:
                for(i = 0; i < ansi_buf_ofs; i++)
                {
                    con_putc(ansi_buf[i]);
                }
                ansi_buf_ofs = 0;
                break;
            
            case SUCCESS:
                ansi_buf_ofs = 0;
                break;
        }
    }
    else
    {
        con_putc(c);
    }
}

/**
 * Leert den Puffer, in dem die Zeichen einer m�glichen ANSI-Escape-Code-
 * Sequenz zwischengespeichert werden. Diese Funktion muss aufgerufen werden,
 * wenn sichergestellt werden soll, dass s�mtliche Zeichen, die via 
 * con_putc_ansi ausgegeben werden sollen, tats�chlich ausgegeben werden.
 */
static void con_flush_ansi_escape_code_sequence(void)
{
    int i;
    
    if(ansi_buf_ofs > 0)
    {
        for(i = 0; i < ansi_buf_ofs; i++)
        {
            con_putc(ansi_buf[i]);
        }
        ansi_buf_ofs = 0;
    }
}

/**
 * Gibt eine Zeichenkette auf der Konsole aus. Diese Zeichenkette kann
 * ANSI-Steuercodes enthalten. Die Ausgabe terminiert bei Erreichen eines
 * Null-Bytes oder wenn n Bytes ausgewertet wurden.
 * Unvollst�ndige und fehlerhafte Sequenzen werden einfach ausgegeben.
 */
static ACE_size_t console_write(MFS_stream_t *stream, const char *s, ACE_size_t n)
{
	ACE_size_t ret;
	ret = n;
    while(*s && n--)
    {
        con_putc_ansi(*s);
        s++;
    }

    con_flush_ansi_escape_code_sequence();
    return ret;
}

/**
 * Initialisiert die Console, in dem der Bildschirm geleert wird.
 */
static ACE_err_t console_open(MFS_descriptor_t * desc)
{
    con_color._color = 0x07;
    con_clear_screen();
    return ACE_OK;
}

static struct MFS_descriptor_op console_desc_op = {
	.open = console_open,
    .close = NULL,
    .info = NULL,
    .control = NULL
};

static struct MFS_stream_op console_stream_op = {
    .read = NULL,
    .write = console_write,
    .seek = NULL,
    .flush = NULL
};

extern void
IBMPC_console_install (IBMPC_console_t *con, char *name)
{
    MFS_stream_create (MFS_resolve(MFS_get_root(), "sys/dev/serial"), name, &console_desc_op,
                   &console_stream_op, (MFS_represent_t *) con, MFS_STREAM_IO);
}
