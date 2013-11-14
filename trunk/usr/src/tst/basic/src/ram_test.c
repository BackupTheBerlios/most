/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <cli/exec.h>

#include "tst/ram_test.h"

static CLI_exec_t ramt;

static ACE_err_t
ram_test_exec (char *nix)
{
    for (;;)
    {
        if (ACE_malloc (1000) != NULL)
        {
            USO_log_puts (USO_LL_INFO, "1000 B ram allocated.\n");
            USO_sleep (USO_MSEC_2_TICKS (100));
        }
        else
        {
            USO_log_puts (USO_LL_INFO, "Out of ram\n");
            break;
        }
    }
    return ACE_OK;
}

extern void
TST_ram_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &ramt, "ram_T", "Ram Test", ram_test_exec);
}
