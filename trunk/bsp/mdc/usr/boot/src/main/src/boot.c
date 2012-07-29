/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <arch/digio.h>
#include <init/bsp_commands.h>
#include <boot.h>
#include <download_app.h>
#include <download_boot.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, MDC_APPLICATION " " ACE_MOST_VERSION "\n");
    if (DEV_digin_isset (&MDC_switch) == FALSE)
    {
        MDC_start_app ();
    }
    DEV_digout_set (&MDC_red_led);
    MFS_descriptor_t *boot;
    boot = MFS_create_dir (MFS_sysfs_get_dir (MFS_SYSFS_DIR_ROOT), "boot");
    MDC_download_app_install (boot);
    MDC_download_boot_install (boot);
}
