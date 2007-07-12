/*
 *  Copyright (C) 2004-2007  Michael Feiner
 *
 */

#include <ace/arch/cpu.h>

#include "dev/clock.h"

static unsigned long ticks;

extern void
DEV_clock_init (void)
{
    ticks = 0;
}

extern void
DEV_clock_tick (void)
{
    ++ticks;
}

extern unsigned long
DEV_get_ticks (void)
{
    return ticks;
}

extern unsigned long
DEV_get_ticks_diff (unsigned long old_ticks)
{
    return ticks - old_ticks;
}
