#include <uso/log.h>
#include <uso/sleep.h>
#include <mfs/directory.h>
#include <cli/exec.h>
#include <arch/digio.h>

#include <init/bsp_commands.h>
#include <init/net.h>
#include <init/download.h>

#include <boot.h>
#include <err.h>

void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "SAM_APPLICATION"\n");
    if (DEV_digin_isset (&SAM_switch_boot) == FALSE)
    {
        SAM_start_app ();
    }
    DEV_digout_set (&SAM_red_led);

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *boot;
    boot = MFS_directory_create (app, "boot");
    SAM_download_install (boot, SAM_APPL_START, SAM_APPL_END);
    MFS_close_desc(app);
    CLI_ymodem_install();
    SAM_net_start(NULL);
}
