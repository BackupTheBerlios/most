/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <nap/syslog.h>
#include <nap/ymodem.h>

#include <init/net.h>
#include <init/download.h>

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
    USO_log_puts (USO_LL_INFO, "App: "MDC_APPLICATION" -- 2 -- \n");
    USO_log_printf (USO_LL_INFO, "Sizeof(char, short, int, long) = %d %d %d %d.\n", sizeof (char), sizeof (short), sizeof (int), sizeof (long));

    NAP_ymodem_install();
    MDC_net_start(NULL);

    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (MFS_get_root(), "putboot");

    MDC_download_install (putboot, 1, 2);

    MFS_descriptor_t *test;
    test = MFS_directory_create (MFS_get_root(), "test");

    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    flash_test_install (test);
    ee_test_install (test);
    digio_test_install (test);
}
