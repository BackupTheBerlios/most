#ifndef LCD_KS0070B_H_
#define LCD_KS0070B_H_

#include <ace/stddef.h>
#include <ace/stdio.h>

// commands
#define LCD_CLEARDISPLAY     0x01
#define LCD_RETURNHOME       0x02
#define LCD_ENTRYMODESET     0x04
#define LCD_DISPLAYCONTROL   0x08
#define LCD_CURSORSHIFT      0x10
#define LCD_FUNCTIONSET      0x20
#define LCD_SETCGRAMADDR     0x40
#define LCD_SETDDRAMADDR     0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON        0x04
#define LCD_DISPLAYOFF       0x00
#define LCD_CURSORON         0x02
#define LCD_CURSOROFF        0x00
#define LCD_BLINKON          0x01
#define LCD_BLINKOFF         0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE      0x08
#define LCD_CURSORMOVE       0x00
#define LCD_MOVERIGHT        0x04
#define LCD_MOVELEFT         0x00

// flags for function set
#define LCD_8BITMODE         0x10
#define LCD_4BITMODE         0x00
#define LCD_2LINE            0x08
#define LCD_1LINE            0x00
#define LCD_5x10DOTS         0x04
#define LCD_5x8DOTS          0x00

struct  DPY_ks0070b {
    ACE_u8_t function;
    ACE_u8_t control;
    ACE_u8_t mode;
    ACE_bool_t initialized;
    int characters;
    int lines;
    int currline;
    void (*command)(ACE_u8_t value);
    ACE_size_t (*data)(ACE_u8_t value);
};

typedef struct DPY_ks0070b DPY_ks0070b_t;

extern void DPY_ks0070b_init(DPY_ks0070b_t *lcd, int characters, int lines, int dots, int mode,
                     void (*command)(ACE_u8_t value),
                     ACE_size_t (*data)(ACE_u8_t value));

extern void DPY_ks0070b_clear(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_home(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_cursor(DPY_ks0070b_t *lcd, int col, int row);
extern void DPY_ks0070b_display_off(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_display_on(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_cursor_off(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_cursor_on(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_blink_off(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_blink_on(DPY_ks0070b_t *lcd);

// These commands scroll the display without changing the RAM
extern void DPY_ks0070b_scroll_left(DPY_ks0070b_t *lcd);
extern void DPY_ks0070b_scroll_right(DPY_ks0070b_t *lcd);

// This is for text that flows Left to Right
extern void DPY_ks0070b_left(DPY_ks0070b_t *lcd);
// This is for text that flows Right to Left
extern void DPY_ks0070b_right(DPY_ks0070b_t *lcd);
// This will 'right justify' text from the cursor
extern void DPY_ks0070b_autoscroll_on(DPY_ks0070b_t *lcd);
// This will 'left justify' text from the cursor
extern void DPY_ks0070b_autoscroll_off(DPY_ks0070b_t *lcd);

// Allows us to fill the first 8 CGRAM locations
// with custom characters
extern void DPY_ks0070b_create_char(DPY_ks0070b_t *lcd, int location, ACE_u8_t charmap[]);

extern ACE_size_t DPY_ks0070b_put_string(DPY_ks0070b_t *lcd, char *s);


#endif
