/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <arch/digio.h>
#include <init/bsp_commands.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>

#include <init/net.h>
#include <init/download.h>

#include <boot.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION" -- 2 -- \n");
    if (DEV_digin_isset (&MDC_switch) == FALSE)
    {
        MDC_start_app ();
    }
    DEV_digout_set (&MDC_red_led);

    NAP_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *boot;
    boot = MFS_directory_create (MFS_get_root(), "boot");

    MDC_download_install (boot, MDC_APPL_START, MDC_APPL_END);
}
