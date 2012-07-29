/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <uso/sleep.h>
#include <cli/commands.h>
#include <nap/syslog.h>

#include "tst/ser_test.h"

static CLI_exec_t nett;

static void
net_test_exec (char *nix)
{
    for (int i = 0; i < 100; ++i)
    {
        USO_sleep (USO_MSEC_2_TICKS (5000));
        NAP_syslog_printf (NAP_SYSLOG_DEBUG, NAP_SYSLOG_LOCAL1, "Syslog test %d.", i);
    }
}

extern void
TST_net_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &nett, "net_T", "Network Test", net_test_exec);
}
