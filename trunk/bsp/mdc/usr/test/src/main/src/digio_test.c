/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/thread.h>
#include <dev/digin.h>
#include <dev/digout.h>
#include <cli/commands.h>

#include "arch/digio.h"
#include "digio_test.h"

static CLI_exec_t digio_test;

static DEV_digout_t *runninglight[] = {
    &MDC_green_led,
    &MDC_red_led
};

static void
cleanup(void)
{
    for (int i = 0; i < ARRAYSIZE (runninglight); ++i)
    {
	    DEV_digout_clear (runninglight[i]);
    }
}

static void
digio_test_exec (char *nix)
{
    int i;
    USO_cleanup_install(cleanup);
    for (;;)
    {
        while (DEV_digin_isset (&MDC_jumper_1) == FALSE)
        	USO_sleep(ACE_MSEC_2_TICKS(100));
        for (;;)
        {
            for (i = 0; i < ARRAYSIZE (runninglight); ++i)
            {
                DEV_digout_set (runninglight[i]);
                USO_sleep (ACE_MSEC_2_TICKS(50));
                DEV_digout_clear (runninglight[i]);
            }
            while (--i >= 0)
            {
                DEV_digout_set (runninglight[i]);
                USO_sleep (ACE_MSEC_2_TICKS(50));
                DEV_digout_clear (runninglight[i]);
            }
            if (DEV_digin_isset (&MDC_jumper_1) == TRUE)
                break;
        }
    }
}

extern void
digio_test_install(MFS_descriptor_t *test)
{
    CLI_exec_init (test, &digio_test, "digioT", "Digio Test", digio_test_exec);
}
