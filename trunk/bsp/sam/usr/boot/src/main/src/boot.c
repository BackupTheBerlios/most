#include <uso/log.h>
#include <uso/sleep.h>
#include <mfs/directory.h>
#include <nap/syslog.h>
#include <arch/digio.h>
#include <init/bsp_commands.h>
#include <boot.h>
#include <download_app.h>

void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, SAM_APPLICATION " " ACE_MOST_BUILD "\n");
    USO_log_puts (USO_LL_INFO, " -- 4 --\n");      /* Increase number to determine a successful download */
    if (DEV_digin_isset (&SAM_switch_boot) == FALSE)
    {
        SAM_start_app ();
    }
    DEV_digout_set (&SAM_red_led);

    MFS_descriptor_t *boot;
    boot = MFS_create_dir (MFS_sysfs_get_dir (MFS_SYSFS_DIR_ROOT), "boot");

    SAM_download_app_install (boot);
}
