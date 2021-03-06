/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <cli/exec.h>

#include <arch/lcd.h>
#include <init/config.h>
#include <init/net.h>
#include <init/download.h>

#include <frame.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION"\n");

    MDC_lcd_init();
    MDC_lcd_backlight_on(TRUE);
    DPY_ks0070b_put_string(&lcd, MDC_APPLICATION);

    MDC_config_create ();
    CLI_executes_install();
    CLI_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;  /* dir for updating bootloader */
    putboot = MFS_directory_create (app, "putboot");
    MFS_close_desc(app);
    MDC_download_install (putboot, MDC_BOOT_START, MDC_BOOT_END);
}
