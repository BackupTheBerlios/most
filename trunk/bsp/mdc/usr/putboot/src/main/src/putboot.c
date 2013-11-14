/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <nap/ymodem.h>
#include <arch/digio.h>
#include <init/bsp_commands.h>
#include <init/download.h>

#include <putboot.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION"\n");
    if (DEV_digin_isset (&MDC_switch) == FALSE)
        MDC_start_app ();
    if (DEV_digin_isset (&MDC_button) == FALSE)
        MDC_start_boot ();
    DEV_digout_set (&MDC_red_led);

    NAP_ymodem_install();

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");
    MFS_close_desc(app);
    MDC_download_install (putboot, MDC_BOOT_START, MDC_APPL_END);
    MFS_close_desc(app);
}
