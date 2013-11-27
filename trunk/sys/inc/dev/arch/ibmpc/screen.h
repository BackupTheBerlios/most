/*
 * screen.h
 *
 *  Created on: 23.11.2013
 *      Author: maik
 */

#ifndef IBMPC_SCREEN_H_
#define IBMPC_SCREEN_H_

#include <ace/stddef.h>

// Aufschluesselung des Attribut-Bytes vom Text Modus
union IBMPC_screen_attr
{
    unsigned char all;
    struct
    {
        unsigned char foreground : 3;
        unsigned char bold : 1;
        unsigned char background : 3;
        unsigned char blink : 1;
    }bits;
};

typedef union IBMPC_screen_attr IBMPC_screen_attr_t;

struct IBMPC_screen{
    unsigned short *vidmem;
    unsigned int cursor_x;
    unsigned int cursor_y;
    ACE_bool_t line_wrap;
    IBMPC_screen_attr_t attr;
};

typedef struct IBMPC_screen IBMPC_screen_t;
/**
 * Setzt die Cursor-Position
 */
extern void IBMPC_screen_set_cursor_pos(IBMPC_screen_t *screen, unsigned int x, unsigned int y);

extern void IBMPC_screen_get_cursor_pos(IBMPC_screen_t *screen, unsigned int *save_cursor_x, unsigned int *save_cursor_y);

extern void IBMPC_screen_set_cursor_x_rel(IBMPC_screen_t *screen, int n);

extern void IBMPC_screen_set_cursor_y_rel(IBMPC_screen_t *screen, int n);

/**
 * Scrollt eine Zeile nach unten
 */
extern void IBMPC_screen_scroll_down(IBMPC_screen_t *screen);

unsigned char IBMPC_screen_get_attribute(IBMPC_screen_t *screen);

void IBMPC_screen_set_attribute(IBMPC_screen_t *screen, unsigned char attr);

/**
 * Setzt die Attribute fuer die folgenden Ausgabe entsprechend der obigen Liste.
 */
extern ACE_bool_t IBMPC_screen_set_attribute_number(IBMPC_screen_t *screen, int n);

/**
 * Leert den Bildschirm
 */
extern void IBMPC_screen_clear(IBMPC_screen_t *screen);

/**
 * Loescht den Bildschirm von der aktuellen Cursor Position bis zur
 * angegebenen Spalte
 *
 * @return Die Spalte bis zu der geloescht wurde
 */
extern unsigned int IBMPC_screen_clear_to(IBMPC_screen_t *screen, unsigned int column);

extern unsigned int IBMPC_screen_clear_to_end(IBMPC_screen_t *screen);

/**
 * Gibt ein Zeichen auf dem Bildschirm aus.
 */
extern void IBMPC_screen_putc(IBMPC_screen_t *screen, const char c);

extern void IBMPC_screen_init(IBMPC_screen_t *screen);

#endif /* SCREEN_C_ */
