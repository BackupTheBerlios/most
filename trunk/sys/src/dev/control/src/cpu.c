/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dev/cpu.h"

#define DEV_LOOPS_PER_MSEC 1000L        /* Default */

/** this value can be overwritten by the BSP */
unsigned long long DEV_loops_per_msec = DEV_LOOPS_PER_MSEC;

extern void
DEV_cpudelay (unsigned long loop)
{
    volatile unsigned long i = loop;    /* because compiler optimization */
    while (i)
    {
        --i;
    }
}
