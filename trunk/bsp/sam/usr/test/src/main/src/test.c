#include <ace/stdio.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <dev/cpu.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <nap/syslog.h>
#include <dev/arch/at91/rst.h>

#include <tst/ser_test.h>
#include <tst/thread_test.h>
#include <tst/ram_test.h>
#include <tst/net_test.h>

#include "arch/digio.h"
#include "init/init.h"
#include "test.h"
#include "lcd_test.h"
#include "mmc_test.h"
#include "digio_test.h"
#include "debug_test.h"
#include "flash_test.h"


void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, SAM_APPLICATION " " ACE_MOST_BUILD "\n");
    USO_log_puts (USO_LL_INFO, " -- 4 --\n");      /* Increase number to determine a successful download */
    USO_log_printf (USO_LL_INFO, "Sizeof(int) = %d.\n", sizeof (int));

    MFS_descriptor_t *test;
    test = MFS_create_dir (MFS_sysfs_get_dir (MFS_SYSFS_DIR_ROOT), "test");

    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    digio_test_install (test);
    mmc_test_install (test);
    lcd_test_install (test);
    debug_test_install (test);
    flash_test_install (test);

    for (;;)
    {
        USO_sleep (USO_MSEC_2_TICKS (1000));
        if (DEV_at91_RST_is_user_reset_detected (AT91C_BASE_RSTC))
        {
            ACE_printf ("user reset detected!");
        }
    }
}
