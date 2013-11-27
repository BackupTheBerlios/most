/*
 * vt100_output.c
 *
 *  Created on: 23.11.2013
 *      Author: maik
 */

#include <ace/ascii.h>
#include <ace/stdio.h>
#include <ace/esc.h>
#include <dev/err.h>
#include <dev/arch/ibmpc/screen.h>
#include <dev/arch/ibmpc/vt100_output.h>

/*
 * Das hier ist eine sehr beschraenkte Untermenge der ANSI Escape Codes. Alle
 * Sequenzen werden mit dem ASCII Zeichen ESC (Wert 0x1b) eingeleitet. ESC
 * soll im folgenden dieses Zeichen darstellen. Wir koennen folgende Escape
 * Sequenzen behandeln:
 *
 * ESC[#A - Cursor um # Zeilen nach oben bewegen
 * ESC[#B - Cursor um # Zeilen nach unten bewegen
 * ESC[#C - Cursor um # Spalten nach rechts bewegen
 * ESC[#D - Cursor um # Spalten nach links bewegen
 * ESC[2J - Bildschirm leeren
 * ESC[K - Bis Zeile von der Cursor-Position zum Ende leeren
 * ESC[#m oder ESC[#;#m - Vorder- und Hintergrundfarbe aendern, Fettschrift, Blinken
 * ESC[s - Cursor-Position speichern
 * ESC[u - Cursor-Position wiederherstellen
 *
 *
 * Es koennen nur 1 oder 2 Farbcodes pro Escape-Sequenz angewendet werden.
 *
 * Ungueltige und nicht erkannte Sequenzen werden einfach ausgegeben.
 */


extern enum IBMPC_vt100_output_status
IBMPC_vt100_output(IBMPC_console_t *con, const char *buf, unsigned int buf_len)
{
    IBMPC_screen_t *screen = &con->screen;

    int i;
    int n1 = 0, n2 = 0;
    int have_n1 = 0, have_n2 = 0, have_delimiter = 0;

    if(buf_len == 0)
    {
        return IBMPC_VT100_OUT_NEED_MORE;
    }

    if(buf[0] != ACE_ASCII_ESC)
    {
        return IBMPC_VT100_OUT_INVALID;
    }

    if(buf_len == 1)
    {
        return IBMPC_VT100_OUT_NEED_MORE;
    }

    if(buf[1] == 'D')
    {
        IBMPC_screen_scroll_down(screen);
        return IBMPC_VT100_OUT_SUCCESS;
    }

    if(buf[1] != '[')
    {
        return IBMPC_VT100_OUT_INVALID;
    }

    if(buf_len == 2)
    {
        return IBMPC_VT100_OUT_NEED_MORE;
    }


    for(i = 2; i < buf_len; i++)
    {
        switch(buf[i])
        {
            case '0' ... '9':
                if(!have_delimiter)
                {
                    n1 = n1 * 10 + (buf[i] - '0');
                    have_n1 = 1;
                }
                else
                {
                    n2 = n2 * 10 + (buf[i] - '0');
                    have_n2 = 1;
                }
                break; // naechstes Zeichen auswerten

            case ';':
                if(have_delimiter)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                have_delimiter = 1;
                break; // naechstes Zeichen auswerten


            case 'A': // ESC[#A - Cursor um # Zeilen nach oben bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_set_cursor_y_rel(screen, -n1);
                return IBMPC_VT100_OUT_SUCCESS;

            case 'B': // ESC[#B - Cursor um # Zeilen nach unten bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_set_cursor_y_rel(screen, n1);
                return IBMPC_VT100_OUT_SUCCESS;

            case 'C': // ESC[#C - Cursor um # Zeilen nach rechts bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_set_cursor_x_rel(screen, n1);
                return IBMPC_VT100_OUT_SUCCESS;

            case 'D': // ESC[#D - Cursor um # Zeilen nach links bewegen
                if(have_delimiter || !have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_set_cursor_x_rel(screen, n1);
                return IBMPC_VT100_OUT_SUCCESS;

            case 'H': // ESC[#;#H - Cursor zur Zeile # und Spalte # bewegen
            case 'f':
                if(!have_n1 || !have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }

                IBMPC_screen_set_cursor_pos(screen, n2, n1);
                return IBMPC_VT100_OUT_SUCCESS;

            case 'J': // ESC[2J - Bildschirm leeren
                if(have_delimiter || !have_n1 || have_n2 || n1 != 2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_clear(screen);
                return IBMPC_VT100_OUT_SUCCESS;

            case 'K': // ESC[K - Bis Zeile von der Cursor Position zum Ende leeren
                if(have_delimiter || have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }

                IBMPC_screen_clear_to_end(screen);

                return IBMPC_VT100_OUT_SUCCESS;

            case 'h': // ESC[7h - Enable line wrap
                if(!have_n1 || have_n2 || n1 != 7)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }

                screen->line_wrap = TRUE;

                return IBMPC_VT100_OUT_SUCCESS;

            case 'l': // ESC[7l - Disable line wrap
                if(!have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }

                screen->line_wrap = FALSE;

                return IBMPC_VT100_OUT_SUCCESS;

            case 'm': // ESC[#m oder ESC[#;#m - Vorder- und Hintergrundfarbe aendern, Fettschrift, Blinken
            {
                // Attribute sichern
                unsigned char attr = IBMPC_screen_get_attribute(screen);

                if(!have_n1)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }

                if(IBMPC_screen_set_attribute_number(screen, n1) == FALSE)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }

                if(have_n2)
                {
                    if(IBMPC_screen_set_attribute_number(screen, n2) == FALSE)
                    {
                        // Bereits uebernommene Aenderungen rueckgaengig machen
                        IBMPC_screen_set_attribute(screen, attr);
                        return IBMPC_VT100_OUT_INVALID;
                    }
                }

                return IBMPC_VT100_OUT_SUCCESS;
            }

            case 'n':
            {
                if(!have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                switch(n1)
                {
                    case 6:
                    {
                        unsigned int cursor_x;
                        unsigned int cursor_y;
                        char string[IBMPC_CONSOLE_TERM_MSG_SIZE];
                        IBMPC_screen_get_cursor_pos(screen, &cursor_x, &cursor_y);
                        ACE_sprintf(string, ACE_CSI"%u;%uR", cursor_y, cursor_x);
                        IBMPC_console_process_term (con, string);
                        break;
                    }
                    default:
                        return IBMPC_VT100_OUT_INVALID;
                }
                return IBMPC_VT100_OUT_SUCCESS;

            }
            case 's': // ESC[s - Cursor-Position speichern
                if(have_delimiter || have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_get_cursor_pos(screen, &con->save_cursor_x, &con->save_cursor_y);

                return IBMPC_VT100_OUT_SUCCESS;

            case 'u': // ESC[u - Cursor-Position wiederherstellen
                if(have_delimiter || have_n1 || have_n2)
                {
                    return IBMPC_VT100_OUT_INVALID;
                }
                IBMPC_screen_set_cursor_pos(screen, con->save_cursor_x, con->save_cursor_y);

                return IBMPC_VT100_OUT_SUCCESS;

            default:
                return IBMPC_VT100_OUT_INVALID;
        }
    }

    return IBMPC_VT100_OUT_NEED_MORE;
}

