/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <nap/syslog.h>

#include <tst/ram_test.h>
#include <tst/ser_test.h>
#include <tst/thread_test.h>
#include <tst/net_test.h>

#include <test.h>
#include <flash_test.h>
#include <ee_test.h>
#include <digio_test.h>

extern void
MDC_main (void)
{
    USO_log_puts (USO_LL_INFO, MDC_APPLICATION " " ACE_MOST_VERSION "\n");
    USO_log_puts (USO_LL_INFO, "--- 2 ---\n");
    USO_log_printf (USO_LL_INFO, "Sizeof(int) = %d.\n", sizeof (int));

    MFS_descriptor_t *test;
    test = MFS_create_dir (MFS_sysfs_get_dir (MFS_SYSFS_DIR_ROOT), "test");

    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    flash_test_install (test);
    ee_test_install (test);
    digio_test_install (test);
}
