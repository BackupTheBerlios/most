/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <cli/exec.h>

#include <arch/digio.h>
#include <arch/lcd.h>
#include <init/bsp_commands.h>
#include <init/net.h>
#include <init/config.h>
#include <init/download.h>

#include <boot.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION"\n");
    if (DEV_digin_isset (&MDC_switch) == FALSE)
    {
        MDC_start_app ();
    }
    DEV_digout_set (&MDC_red_led);

    MDC_lcd_init();
    MDC_lcd_backlight_on(TRUE);
    DPY_ks0070b_put_string(&lcd, MDC_APPLICATION);

    MDC_config_create ();
    CLI_executes_install();
    CLI_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *boot;
    boot = MFS_directory_create (app, "boot");
    MFS_close_desc(app);
    MDC_download_install (boot, MDC_APPL_START, MDC_APPL_END);
}
