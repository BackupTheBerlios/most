/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>
#include <arch/digio.h>
#include <init/bsp_commands.h>
#include <init/download.h>
#include <init/net.h>

#include <putboot.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION" -- 1 -- \n");
    if (DEV_digin_isset (&MDC_switch) == FALSE)
        MDC_start_app ();
    if (DEV_digin_isset (&MDC_button) == FALSE)
        MDC_start_boot ();
    DEV_digout_set (&MDC_red_led);

    NAP_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (MFS_get_root(), "putboot");

    MDC_download_install (putboot, MDC_BOOT_START, MDC_APPL_END);
}
