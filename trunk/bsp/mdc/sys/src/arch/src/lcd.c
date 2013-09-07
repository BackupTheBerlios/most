/*
 * lcd.c
 *
 *  Created on: 31.08.2013
 *      Author: maik
 */

#include <arch/digio.h>
#include <arch/lcd.h>
#include <uso/sleep.h>

DPY_ks0070b_t lcd;

static void send_command(ACE_u8_t value)
{
    static ACE_u8_t *command_addr = (ACE_u8_t *)0xC00000; // CS6, E0, Addr 0
    *command_addr = value;
    USO_sleep(USO_MSEC_2_TICKS(1)); 
}

static ACE_size_t send_data(ACE_u8_t value)
{
    static ACE_u8_t *data_addr = (ACE_u8_t *)0xC00001; // CS6, E0, Addr 1
    *data_addr = value;
    USO_sleep(USO_MSEC_2_TICKS(1)); 
    return 1; // assume sucess
}

extern void
MDC_lcd_init (void)
{
    if (lcd.initialized == FALSE){
        DPY_ks0070b_init(&lcd, 16, 2, 8, 8, send_command, send_data);
    } else {
        DPY_ks0070b_clear(&lcd);
    }
}


extern void
MDC_lcd_backlight_on(ACE_bool_t on)
{
    if (on == TRUE){
        DEV_digout_set(&MDC_lcd_backlight);
    } else {
        DEV_digout_clear(&MDC_lcd_backlight);
    }
}
