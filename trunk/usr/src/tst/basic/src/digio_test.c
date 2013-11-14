/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/thread.h>
#include <cli/exec.h>

#include <tst/digio_test.h>

static CLI_exec_t digio_test;

static DEV_digin_t *stop;

static DEV_digout_t **runninglight;

extern void
TST_digio_test_init (DEV_digin_t *in, DEV_digout_t *out[])
{
    stop = in;
    runninglight = out;
}

static void
cleanup (void *nix)
{
    for (int i = 0; runninglight[i] != NULL; ++i)
    {
        DEV_digout_clear (runninglight[i]);
    }
}

static ACE_err_t
digio_test_exec (char *nix)
{
    int i;
    USO_cleanup_install (cleanup);
    for (;;)
    {
        for (i = 0; runninglight[i] != NULL; ++i)
        {
            DEV_digout_set (runninglight[i]);
            USO_sleep (USO_MSEC_2_TICKS (50));
            DEV_digout_clear (runninglight[i]);
        }
        while (--i >= 0)
        {
            DEV_digout_set (runninglight[i]);
            USO_sleep (USO_MSEC_2_TICKS (50));
            DEV_digout_clear (runninglight[i]);
        }
        if (DEV_digin_isset (stop) == TRUE)
            break;
    }
    return ACE_OK;
}

extern void
TST_digio_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &digio_test, "digio_T", "Digio Test", digio_test_exec);
}
