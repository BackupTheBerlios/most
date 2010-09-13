/*
 * debug_test.c
 *
 *  Created on: 11.09.2010
 *      Author: maik
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/arch/cpu.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <cli/commands.h>

static CLI_exec_t debug_test;
static CLI_exec_t exception_test;

static void debug_test_cycle(void) ACE_SECTION_ (".ramcode");
static void debug_test_cycle(void)
{
	static int cycle = 0;
    int a, b;
    a = cycle;
    ++cycle;
    b = cycle;
    a += b;
}

static void debug_test_exec(char *nix)
{
	for(int i = 0; i < 100; ++i){
		debug_test_cycle();
		USO_sleep(ACE_MSEC_2_TICKS(1000));
	}
}

static void exception_test_exec(char *nix)
{
	int *pointer=(int*)0x87654321, value;
	value = *pointer;
}

extern void
debug_test_install(MFS_descriptor_t *test)
{
    CLI_exec_init (test, &debug_test, "debug_T", "Debug Test", debug_test_exec);
    CLI_exec_init (test, &exception_test, "excep_T", "Exception Test", exception_test_exec);
}
