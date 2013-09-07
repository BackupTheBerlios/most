/*
 * lcd_bl_test.c
 *
 *  Created on: 24.08.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/exec.h>

#include <arch/lcd.h>
#include <lcd_bl_test.h>

static CLI_exec_t backlight;

static void
backlight_exec (char *arg)
{
    if (arg[0] == '+'){
        MDC_lcd_backlight_on(TRUE);
    } else if (arg[0] == '-'){
        MDC_lcd_backlight_on(FALSE);
    } else {
        ACE_printf("use +(on)|-(off)");
    }
}

extern void
lcd_bl_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &backlight, "lcd_BL", "LCD backlight +(on)/-(off)", backlight_exec);
}

