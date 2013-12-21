/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <uso/event.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/exec.h>

#include <arch/digio.h>
#include <arch/lcd.h>
#include <init/net.h>
#include <init/config.h>
#include <init/download.h>
#include <init/events.h>

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

#define TEST_SLOT_SIZE 3


static USO_slot_t test_slot;

static const long test_event_ids[] = {
                MDC_red_pressed,
                MDC_red_released,
                MDC_green_pressed,
                MDC_green_released,
                MDC_blue_pressed,
                MDC_blue_released,
                MDC_yellow_pressed,
                MDC_yellow_released,
                0
};

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
    
    MDC_config_create ();
    CLI_executes_install();
    CLI_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");

    MDC_download_install (putboot, MDC_BOOT_START, MDC_BOOT_END);

    MFS_descriptor_t *test;
    test = MFS_directory_create (app, "test");
    MFS_close_desc(app);

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

    USO_slot_init (&MDC_event_slots, &test_slot, test_event_ids, TEST_SLOT_SIZE);

    for (;;)
    {
        ACE_bool_t free;
        USO_node_t *e = USO_slot_get_event (&test_slot);
        enum MDC_event event = e->data;

        switch (event)
        {
        case MDC_red_pressed:
            ACE_printf("red pressed\n");
            break;
        case MDC_red_released:
            ACE_printf("red_released\n");
            break;
        case MDC_green_pressed:
            ACE_printf("green pressed\n");
            break;
        case MDC_green_released:
            ACE_printf("green_released\n");
            break;
        case MDC_blue_pressed:
            ACE_printf("blue pressed\n");
            break;
        case MDC_blue_released:
            ACE_printf("blue_released\n");
            break;
        case MDC_yellow_pressed:
            ACE_printf("yellow pressed\n");
            break;
        case MDC_yellow_released:
            ACE_printf("yellow_released\n");
            break;
        default:
            ACE_printf("unknown event\n");
            break;
        }

        free = USO_slots_put_event (&MDC_event_slots, &test_slot, e, FALSE);

        if (free == FALSE)
            USO_buf_free (&MDC_event_pool, e);
    }

}
