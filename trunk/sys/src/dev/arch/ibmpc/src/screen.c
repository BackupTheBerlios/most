/*
 * screen.c
 *
 *  Created on: 23.11.2013
 *      Author: maik
 */

#include <ace/string.h>
#include <uso/io_ports.h>
#include <dev/arch/ibmpc/screen.h>
#include <dev/err.h>

/*
 * - Attribute:
 *  0 - Normale Schrift
 *  1 - Fett
 *  5 - Blinken
 * - Vordergrundfarben:
 *  30 - Schwarz
 *  31 - Rot
 *  32 - Gruen
 *  33 - Gelb
 *  34 - Blau
 *  35 - Magenta
 *  36 - Cyan
 *  37 - Weiss
 * - Hintergrundfarben:
 *  40 - Schwarz
 *  41 - Rot
 *  42 - Gruen
 *  43 - Gelb
 *  44 - Blau
 *  45 - Magenta
 *  46 - Cyan
 *  47 - Weiss
 *
 */


#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

/**
 * Aktualisiert die Postition des Hardware Cursors
 */
static void
screen_set_hw_cursor(IBMPC_screen_t *screen)
{
    //Hardware Cursor verschieben
    ACE_u16_t hw_cursor_pos = screen->cursor_x + screen->cursor_y * SCREEN_WIDTH;
    USO_out_b(0x3D4, 15);
    USO_out_b(0x3D5, hw_cursor_pos);
    USO_out_b(0x3D4, 14);
    USO_out_b(0x3D5, hw_cursor_pos >> 8);
}

extern void
IBMPC_screen_set_cursor_pos(IBMPC_screen_t *screen, unsigned int x, unsigned int y)
{
    if(x >= SCREEN_WIDTH)
    {
        screen->cursor_x = SCREEN_WIDTH - 1;
    }
    else
    {
        screen->cursor_x = x;
    }

    if(y >= SCREEN_HEIGHT)
    {
        screen->cursor_y = SCREEN_HEIGHT - 1;
    }
    else
    {
        screen->cursor_y = y;
    }

    screen_set_hw_cursor(screen);

}
extern void
IBMPC_screen_get_cursor_pos(IBMPC_screen_t *screen, unsigned int *save_cursor_x, unsigned int *save_cursor_y)
{
    *save_cursor_x = screen->cursor_x;
    *save_cursor_y = screen->cursor_y;
}

extern void
IBMPC_screen_set_cursor_x_rel(IBMPC_screen_t *screen, int n)
{
    IBMPC_screen_set_cursor_pos(screen, screen->cursor_x + n, screen->cursor_y);
}

extern void
IBMPC_screen_set_cursor_y_rel(IBMPC_screen_t *screen, int n)
{
    IBMPC_screen_set_cursor_pos(screen, screen->cursor_x, screen->cursor_y + n);
}

extern void
IBMPC_screen_scroll_down(IBMPC_screen_t *screen)
{
    int i;

    memmove(screen->vidmem, screen->vidmem + SCREEN_WIDTH, SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2);

    for(i = SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        screen->vidmem[i] = screen->attr.all << 8;
    }

    IBMPC_screen_set_cursor_pos(screen, screen->cursor_x, screen->cursor_y - 1);
}

unsigned char
IBMPC_screen_get_attribute(IBMPC_screen_t *screen)
{
    return screen->attr.all;
}

void
IBMPC_screen_set_attribute(IBMPC_screen_t *screen, unsigned char attr)
{
    screen->attr.all = attr;
}

extern ACE_bool_t
IBMPC_screen_set_attribute_number(IBMPC_screen_t *screen, int n)
{
    static char colors[8] = { 0, 4, 2, 6, 1, 5, 3, 7 }; /* Zum Umrechnen der ANSI Farbcodes in die VGA Palette. */

    switch(n)
    {
        case 0: // Normal
            screen->attr.bits.bold = 0;
            screen->attr.bits.blink = 0;
            screen->attr.bits.foreground = 7;
            screen->attr.bits.background = 0;
            break;

        case 1: // Fett
            screen->attr.bits.bold = 1;
            break;

        case 5: // Blinken
            screen->attr.bits.blink = 1;
            break;

        case 30 ... 37: // Vordergrundfarbe
            screen->attr.bits.foreground = colors[n - 30];
            break;

        case 40 ... 47: // Hintergrundfarbe
            screen->attr.bits.background = colors[n - 40];
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

extern void
IBMPC_screen_clear(IBMPC_screen_t *screen)
{
    int i;

    for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        screen->vidmem[i] = screen->attr.all << 8;
    }

    IBMPC_screen_set_cursor_pos(screen, 0, 0);
}

extern unsigned int
IBMPC_screen_clear_to(IBMPC_screen_t *screen, unsigned int column)
{
    int i;

    for(i = screen->cursor_x; i < column; i++)
    {
        screen->vidmem[screen->cursor_y * SCREEN_WIDTH + i] = screen->attr.all << 8;
    }

    return column;
}

extern unsigned int
IBMPC_screen_clear_to_end(IBMPC_screen_t *screen)
{
    return IBMPC_screen_clear_to(screen, SCREEN_WIDTH);
}

/**
 * Gibt ein Zeichen auf dem Bildschirm aus.
 */
extern void
IBMPC_screen_putc(IBMPC_screen_t *screen, const char c)
{
    static char last_char = 0;

    switch(c)
    {
        case '\n':
            if (last_char != '\r') {
                IBMPC_screen_clear_to_end(screen);
            }
            screen->cursor_x = 0;
            screen->cursor_y++;
            break;

        case '\r':
            screen->cursor_x = 0;
            break;

        case '\t':
            screen->cursor_x = IBMPC_screen_clear_to(screen, (screen->cursor_x & ~7) + 8);
            break;

        case '\b':
            if (screen->cursor_x > 0) screen->cursor_x--;
            break;
        default:
            screen->vidmem[screen->cursor_x + screen->cursor_y * SCREEN_WIDTH] = c | (screen->attr.all << 8);
            screen->cursor_x++;
            break;
    }

    if(screen->cursor_x >= SCREEN_WIDTH)
    {
        if (screen->line_wrap == TRUE){
            screen->cursor_y += screen->cursor_x / SCREEN_WIDTH;
            screen->cursor_x %= SCREEN_WIDTH;
        } else {
            screen->cursor_x = SCREEN_WIDTH - 1;
        }
    }


    while(screen->cursor_y >= SCREEN_HEIGHT)
    {
        IBMPC_screen_scroll_down(screen);
    }
    screen_set_hw_cursor(screen);

    last_char = c;
}

extern void
IBMPC_screen_init(IBMPC_screen_t *screen)
{
    screen->vidmem = (unsigned short*)0xb8000;
    screen->attr.all = 0x07;
    screen->line_wrap = FALSE;
    IBMPC_screen_clear(screen);
}
