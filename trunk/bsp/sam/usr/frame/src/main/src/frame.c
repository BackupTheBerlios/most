#include <uso/log.h>
#include <uso/sleep.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <nap/ymodem.h>

#include <init/net.h>
#include <init/download.h>

#include <frame.h>
#include <err.h>

void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "SAM_APPLICATION"\n");

    MFS_descriptor_t *app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");

    SAM_download_install (putboot, SAM_BOOT_START, SAM_BOOT_END);
    NAP_ymodem_install();
    SAM_net_start(NULL);

}
