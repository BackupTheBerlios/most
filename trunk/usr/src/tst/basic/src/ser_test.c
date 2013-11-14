/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>
#include <cli/exec.h>

#include "tst/ser_test.h"

static CLI_exec_t sert;

static ACE_err_t
ser_test_exec (char *nix)
{
    int i;
    for (i = 100; i; --i)
    {
        USO_log_printf (USO_LL_INFO, "Serial Test %d.\n", i);
    }
    return ACE_OK;
}

extern void
TST_ser_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &sert, "ser_T", "Serial Test", ser_test_exec);
}
