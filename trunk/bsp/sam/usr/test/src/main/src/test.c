#include <uso/log.h>
#include <uso/sleep.h>
#include <dev/arch/cpu.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include <tst/ser_test.h>
#include <tst/net_test.h>
#include <tst/thread_test.h>
#include <tst/ram_test.h>

#include "arch/digio.h"
#include "init/init.h"
#include "test.h"
#include "lcd_test.h"
#include "mmc_test.h"
#include "digio_test.h"
#include "debug_test.h"


void SAM_main(void)
{
    USO_kputs (USO_LL_INFO, SAM_APPLICATION" "ACE_MOST_VERSION"\n");
    USO_kprintf(USO_LL_INFO, "Sizeof(int) = %d.\n", sizeof(int));

	MFS_descriptor_t *test;
	test = MFS_create_dir(MFS_sysfs_root(), "test");

    TST_ram_test_install(test);
    TST_ser_test_install(test);
    TST_thread_test_install(test);
    TST_net_test_install(test);
    digio_test_install(test);
    mmc_test_install(test);
    lcd_test_install(test);
    debug_test_install(test);
}
