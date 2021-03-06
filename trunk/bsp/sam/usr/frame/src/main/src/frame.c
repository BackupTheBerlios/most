#include <uso/log.h>
#include <uso/sleep.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <cli/exec.h>

#include <init/net.h>
#include <init/download.h>

#include <frame.h>
#include <err.h>

void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "SAM_APPLICATION"\n");

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");
    SAM_download_install (putboot, SAM_BOOT_START, SAM_BOOT_END);
    MFS_close_desc(app);
    CLI_executes_install();
    CLI_ymodem_install();
    SAM_net_start(NULL);

}
