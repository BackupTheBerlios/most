/*
 * lcd_test.c
 *
 *  Created on: 25.08.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/exec.h>
#include <dev/cpu.h>

#include <arch/lcd.h>
#include <lcd_test.h>

static CLI_exec_t lcd_test;


static void
lcd_exec (char *arg)
{
    MDC_lcd_init();
    DPY_ks0070b_put_string(&lcd, "Hello world!");
}

extern void
lcd_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &lcd_test, "lcd_T", "test lcd", lcd_exec);
}

