/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>

#include <init/net.h>
#include <init/download.h>

#include <frame.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION" -- 1 -- \n");

    NAP_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");

    MDC_download_install (putboot, MDC_BOOT_START, MDC_BOOT_END);
}
