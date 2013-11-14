/*
 * debug_test.c
 *
 *  Created on: 11.09.2010
 *      Author: maik
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <ace/assert.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <cli/exec.h>

#include <tst/debug_test.h>

static CLI_exec_t debug_test;
static CLI_exec_t exception_test;
static CLI_exec_t assert_test;

ACE_SECTION_ (".ramcode")
static void
ACE_LONG_CALL_ debug_test_cycle (void) 
{
    static int cycle = 0;
    int a, b;
    a = cycle;
    ++cycle;
    b = cycle;
    a += b;
} 

static ACE_err_t
debug_test_exec (char *nix)
{
    for (int i = 0; i < 100; ++i)
    {
        debug_test_cycle ();
        USO_sleep (USO_MSEC_2_TICKS (1000));
    }
    return ACE_OK;
}


static ACE_err_t
exception_test_exec (char *arg)
{
    ACE_err_t err = ACE_OK;
    if (arg != NULL){
        if (arg[0] == 'a'){
            int value;
            volatile int *nirvana = (int *)0x87654321;
            value = *nirvana;
            ACE_printf ("abort: %i\n", value);
        }
        if (arg[0] == 'p'){
            int value;
            int (*f) (void);
            f = (int (*) (void))(0x87654321);
            value = f();
            ACE_printf ("prefetch: %i\n", value);
        }
        if (arg[0] == 'u'){
            int value;
            int (*f) (void);
            f = (int (*) (void))(0x13F000);
            value = f();
            ACE_printf ("undefined: %i\n", value);
        }
    } else {
        ACE_puts ("u (undefined 2), p (prefetch 4), a (abort 5)\n");
    }
    return err;
}

static ACE_err_t
assert_test_exec (char *arg)
{
    ACE_err_t err = ACE_OK;
    if (arg != NULL){
        if (arg[0] == 'a'){
            ACE_ASSERT (0, "Assert test\n");
        }
        if (arg[0] == 'p'){
            ACE_PANIC ("Panik test\n");
        }
    } else {
        ACE_puts ("a ASSERT, p PANIK\n");
    }
    return err;
}

extern void
TST_debug_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &debug_test, "debug_T", "Debug Test", debug_test_exec);
    CLI_exec_init (test, &exception_test, "excep_T", "Exception Test", exception_test_exec);
    CLI_exec_init (test, &assert_test, "asser_T", "Assert Test", assert_test_exec);
}
