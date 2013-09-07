/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>

#include <arch/digio.h>
#include <arch/lcd.h>
#include <init/net.h>
#include <init/download.h>

#include <tst/ram_test.h>
#include <tst/ser_test.h>
#include <tst/thread_test.h>
#include <tst/net_test.h>
#include <tst/digio_test.h>
#include <tst/debug_test.h>

#include <test.h>
#include <flash_test.h>
#include <ee_test.h>
#include <lcd_bl_test.h>
#include <rand_test.h>
#include <lcd_test.h>

static DEV_digout_t *runninglight[] = {
    &MDC_green_led,
    &MDC_red_led,
    NULL
};

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION"\n");
    USO_log_printf (USO_LL_INFO, "Sizeof(char, short, int, long) = %d %d %d %d.\n", sizeof (char), sizeof (short), sizeof (int), sizeof (long));

    MDC_lcd_init();
    MDC_lcd_backlight_on(TRUE);
    DPY_ks0070b_put_string(&lcd, MDC_APPLICATION);
    
    NAP_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");

    MDC_download_install (putboot, MDC_BOOT_START, MDC_BOOT_END);

    MFS_descriptor_t *test;
    test = MFS_directory_create (app, "test");

    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    TST_debug_test_install (test);
    TST_digio_test_init (&MDC_button ,runninglight);
    TST_digio_test_install (test);
    flash_test_install (test);
    ee_test_install (test);
    lcd_bl_test_install (test);
    rand_test_install (test);
    lcd_test_install (test);
}
