/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdio.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/scheduler.h>
#include <uso/thread.h>
#include <uso/log.h>
#include <cli/commands.h>

#include "tst/thread_test.h"

static CLI_exec_t thread_test;
static CLI_exec_t scheduler_test;

static void
thread_test_exec (char *text)
{
    for (;;)
    {
        USO_kprintf(USO_LL_INFO, "%s\n", text ? text : "threadt");
        for (unsigned long i = 0; i < 1000000; ++i){;}
    }
}

#define SCHEDULE_COUNT 100
#define SLEEP_TIME_MSEC 10

static void
scheduler_test_exec (char *nix)
{
    unsigned long ticks = USO_current()->ticks;
    for (int i = 0; i < SCHEDULE_COUNT; ++ i)
    {
        USO_sleep(ACE_MSEC_2_TICKS(SLEEP_TIME_MSEC));
    }
    ACE_printf("Sched time usec: %lu\n", (ACE_TICKS_2_MSEC(USO_current()->ticks - ticks) * 1000) / SCHEDULE_COUNT);
}

extern void
TST_thread_test_install(MFS_descriptor_t *test)
{
    CLI_exec_init (test, &thread_test, "thrad_T", "Thread Test", thread_test_exec);
    CLI_exec_init (test, &scheduler_test, "sched_T", "Scheduler Test", scheduler_test_exec);
}
