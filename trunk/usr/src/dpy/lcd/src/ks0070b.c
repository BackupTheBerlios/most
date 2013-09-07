#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stddef.h>
#include <uso/sleep.h>

#include <dpy/ks0070b.h>

extern void DPY_ks0070b_init(DPY_ks0070b_t *lcd, int characters, int lines, int dots, int mode,
                     void (*command)(ACE_u8_t value),
                     ACE_size_t (*data)(ACE_u8_t value))
{
    USO_sleep(USO_MSEC_2_TICKS(50)); 

    lcd->command = command;
    lcd->data = data;
    lcd->characters = characters;
    lcd->lines = lines;
    lcd->currline = 0;
    
    lcd->function = (mode == 4 ? LCD_4BITMODE : LCD_8BITMODE) |
                    (lines = 2 ? LCD_2LINE : LCD_1LINE) |
                    (dots == 8 ? LCD_5x8DOTS: LCD_5x10DOTS);

    lcd->command(LCD_FUNCTIONSET | lcd->function);
    USO_sleep(USO_MSEC_2_TICKS(5)); 

    // second try
    lcd->command(LCD_FUNCTIONSET | lcd->function);
    USO_sleep(USO_MSEC_2_TICKS(2)); 

    // third go
    lcd->command(LCD_FUNCTIONSET | lcd->function);

    lcd->control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
    DPY_ks0070b_display_on(lcd);
    DPY_ks0070b_clear(lcd);

    lcd->mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcd->command(LCD_ENTRYMODESET | lcd->mode);
    lcd->initialized = TRUE;
}

extern void DPY_ks0070b_clear(DPY_ks0070b_t *lcd)
{
    lcd->command(LCD_CLEARDISPLAY);   // clear display, set cursor position to zero
    USO_sleep(USO_MSEC_2_TICKS(2)); 
}

extern void DPY_ks0070b_home(DPY_ks0070b_t *lcd)
{
    lcd->command(LCD_RETURNHOME);  // set cursor position to zero
    USO_sleep(USO_MSEC_2_TICKS(2)); 
}

extern void DPY_ks0070b_cursor(DPY_ks0070b_t *lcd, int col, int row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if ( row >= lcd->lines ) {
        row = lcd->lines-1;    // we count rows starting w/0
    }
    lcd->command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}


extern void DPY_ks0070b_display_off(DPY_ks0070b_t *lcd)
{
    lcd->control &= ~LCD_DISPLAYON;
    lcd->command(LCD_DISPLAYCONTROL | lcd->control);
}

extern void DPY_ks0070b_display_on(DPY_ks0070b_t *lcd)
{
    lcd->control |= LCD_DISPLAYON;
    lcd->command(LCD_DISPLAYCONTROL | lcd->control);
}


extern void DPY_ks0070b_cursor_off(DPY_ks0070b_t *lcd)
{
    lcd->control &= ~LCD_CURSORON;
    lcd->command(LCD_DISPLAYCONTROL | lcd->control);
}

extern void DPY_ks0070b_cursor_on(DPY_ks0070b_t *lcd)
{
    lcd->control |= LCD_CURSORON;
    lcd->command(LCD_DISPLAYCONTROL | lcd->control);
}


extern void DPY_ks0070b_blink_off(DPY_ks0070b_t *lcd)
{
    lcd->control &= ~LCD_BLINKON;
    lcd->command(LCD_DISPLAYCONTROL | lcd->control);
}

extern void DPY_ks0070b_blink_on(DPY_ks0070b_t *lcd)
{
    lcd->control |= LCD_BLINKON;
    lcd->command(LCD_DISPLAYCONTROL | lcd->control);
}


// These commands scroll the display without changing the RAM
extern void DPY_ks0070b_scroll_left(DPY_ks0070b_t *lcd)
{
    lcd->command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

extern void DPY_ks0070b_scroll_right(DPY_ks0070b_t *lcd)
{
    lcd->command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}


// This is for text that flows Left to Right
extern void DPY_ks0070b_left(DPY_ks0070b_t *lcd)
{
    lcd->mode |= LCD_ENTRYLEFT;
    lcd->command(LCD_ENTRYMODESET | lcd->mode);
}

// This is for text that flows Right to Left
extern void DPY_ks0070b_right(DPY_ks0070b_t *lcd)
{
    lcd->mode &= ~LCD_ENTRYLEFT;
    lcd->command(LCD_ENTRYMODESET | lcd->mode);
}

// This will 'right justify' text from the cursor
extern void DPY_ks0070b_autoscroll_on(DPY_ks0070b_t *lcd)
{
    lcd->mode |= LCD_ENTRYSHIFTINCREMENT;
    lcd->command(LCD_ENTRYMODESET | lcd->mode);
}

// This will 'left justify' text from the cursor
extern void DPY_ks0070b_autoscroll_off(DPY_ks0070b_t *lcd)
{
    lcd->mode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcd->command(LCD_ENTRYMODESET | lcd->mode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
extern void DPY_ks0070b_create_char(DPY_ks0070b_t *lcd, int location, ACE_u8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    lcd->command(LCD_SETCGRAMADDR | (location << 3));
    for (int i=0; i<8; i++) {
        lcd->data(charmap[i]);
    }
}




extern ACE_size_t DPY_ks0070b_put_string(DPY_ks0070b_t *lcd, char *s)
{
    int r = 0;
    for (int i = 0;*s != '\0' && i < lcd->characters; ++s, ++i){
        r += lcd->data((ACE_u8_t)*s);
    }
    return r;
}
