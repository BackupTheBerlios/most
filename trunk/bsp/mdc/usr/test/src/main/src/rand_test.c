/*
 * rand_test.c
 *
 *  Created on: 24.08.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <cli/exec.h>

#include <arch/digio.h>
#include <arch/rand.h>
#include <rand_test.h>

#define MAX_NUMBERS   45
#define BITS          32

static CLI_exec_t rand;

static unsigned long hw_counter[MAX_NUMBERS];
static unsigned long sw_counter[MAX_NUMBERS];

static int
calc_i(ACE_u32_t r)
{
    return r % MAX_NUMBERS;
}

static void
rand_exec (char *arg)
{
    ACE_u32_t hwr, swr;
    long c = ACE_atol(arg); 
    ACE_seed(0);
    for (int i = 0 ; i < MAX_NUMBERS; ++i){
        hw_counter[i] = 0;
        sw_counter[i] = 0;
    }
    for (long j = 0; j < c; ++j)
    {
        hwr = MDC_rand(BITS, 0);
        swr = ACE_rand();
        ++hw_counter[calc_i(hwr)];
        ++sw_counter[calc_i(swr)];
    }
    for (int i = 0 ; i < MAX_NUMBERS; ++i){
        ACE_printf("N: %i  HW-P: %lu    SW-P: %lu\n", i, hw_counter[i], sw_counter[i]);
    }
}

extern void
rand_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &rand, "rand_T", "print random number", rand_exec);
}


