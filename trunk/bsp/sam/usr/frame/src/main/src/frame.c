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
    USO_log_puts (USO_LL_INFO, "App: "SAM_APPLICATION" -- 1 -- \n");

    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (MFS_get_root(), "putboot");

    SAM_download_install (putboot, SAM_BOOT_START, SAM_APPL_START);
    NAP_ymodem_install();
    SAM_net_start(NULL);

    MFS_descriptor_t *test;
    test = MFS_directory_create (MFS_get_root(), "app");
}
