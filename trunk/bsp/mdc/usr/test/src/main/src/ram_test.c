/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <cli/commands.h>

#include "ram_test.h"

static CLI_exec_t ramt;

static void
ram_test_exec (char *nix)
{
    for (;;)
    {
        if (malloc (1000) != NULL){
        	USO_kputs (USO_LL_INFO, "1000 B ram allocated.\n");
        	USO_sleep (ACE_MSEC_2_TICKS(100));
        } else {
        	USO_kputs (USO_LL_INFO, "Out of ram\n");
        	break;
        }	
    }
}

extern void
ram_test_install(MFS_descriptor_t *test)
{
    CLI_exec_init (test, &ramt, "ramT", "Ram Test", ram_test_exec);
}
