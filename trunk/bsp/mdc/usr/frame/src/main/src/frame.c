/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>

#include <init/net.h>
#include <init/download.h>
#include <arch/lcd.h>

#include <frame.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION"\n");

    MDC_lcd_init();
    MDC_lcd_backlight_on(TRUE);
    DPY_ks0070b_put_string(&lcd, MDC_APPLICATION);

    NAP_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");
    MDC_download_install (putboot, MDC_BOOT_START, MDC_BOOT_END);
    MFS_close_desc(app);
}
