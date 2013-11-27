#include <ace/stdio.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <dev/cpu.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/exec.h>

#include <arch/digio.h>
#include <init/net.h>
#include <init/download.h>

#include <tst/ser_test.h>
#include <tst/thread_test.h>
#include <tst/ram_test.h>
#include <tst/net_test.h>
#include <tst/mmc_test.h>
#include <tst/digio_test.h>
#include <tst/debug_test.h>

#include <test.h>
#include <err.h>
#include <flash_test.h>
#include <lcd_test.h>

static DEV_digout_t *runninglight[] = {
    &SAM_green_led,
    &SAM_red_led,
    NULL
};

void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, "App: "SAM_APPLICATION"\n");
    USO_log_printf (USO_LL_INFO, "Sizeof(char, short, int, long) = %d %d %d %d.\n", sizeof (char), sizeof (short), sizeof (int), sizeof (long));

    MFS_descriptor_t * app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot"); /* is for updating the bootloader */

    SAM_download_install (putboot, SAM_BOOT_START, SAM_BOOT_END);

    CLI_executes_install();
    CLI_ymodem_install();
    SAM_net_start(NULL);

    MFS_descriptor_t *test;
    test = MFS_directory_create (app, "test");
    MFS_close_desc(app);

    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    TST_debug_test_install (test);
    TST_mmc_test_install (test);
    TST_digio_test_init (&SAM_switch_1 ,runninglight);
    TST_digio_test_install (test);
    lcd_test_install (test);
    flash_test_install (test);
}
