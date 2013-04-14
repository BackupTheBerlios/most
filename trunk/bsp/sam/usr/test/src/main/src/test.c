#include <ace/stdio.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <dev/cpu.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>

#include <init/net.h>
#include <init/download.h>

#include <test.h>
#include <err.h>

#include <tst/ser_test.h>
#include <tst/thread_test.h>
#include <tst/ram_test.h>
#include <tst/net_test.h>

#include <lcd_test.h>
#include <mmc_test.h>
#include <digio_test.h>
#include <debug_test.h>
#include <flash_test.h>


void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "SAM_APPLICATION" -- 5 -- \n");
    USO_log_printf (USO_LL_INFO, "Sizeof(char, short, int, long) = %d %d %d %d.\n", sizeof (char), sizeof (short), sizeof (int), sizeof (long));

    MFS_descriptor_t * app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");

    SAM_download_install (putboot, SAM_BOOT_START, SAM_BOOT_END);
    NAP_ymodem_install();
    SAM_net_start(NULL);

    MFS_descriptor_t *test;
    test = MFS_directory_create (app, "test");

    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    digio_test_install (test);
    mmc_test_install (test);
    lcd_test_install (test);
    debug_test_install (test);
    flash_test_install (test);
}
