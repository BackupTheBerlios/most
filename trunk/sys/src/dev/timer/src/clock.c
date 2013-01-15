/*
 *  Copyright (C) 2004-2007  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <ace/cpu.h>
#include <ace/err.h>
#include <uso/sleep.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include "dev/clock.h"

static struct DEV_clock
{
    unsigned long ticks;
    unsigned long (*get_us) (void);
} clock;

static void
info (MFS_descriptor_t * desc)
{
    ACE_printf ("f = %d HZ, ticks: %lu, usec: %d\n", USO_MSEC_2_TICKS (1000), clock.ticks,
                DEV_get_usec ());
}

static struct MFS_descriptor_op clock_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
};


extern void
DEV_clock_init (unsigned long (*get_usec) (void))
{
    clock.ticks = 0;
    clock.get_us = get_usec;
    MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/dev/clock"), "clock",
                     MFS_SYS, &clock_descriptor_op, (MFS_represent_t *) &clock);
}

extern void
DEV_clock_tick (void)
{
    ++clock.ticks;
}

extern unsigned long
DEV_get_ticks (void)
{
    return clock.ticks;
}

extern unsigned long
DEV_get_ticks_diff (unsigned long old_ticks)
{
    return clock.ticks - old_ticks;
}

extern unsigned long
DEV_get_usec (void)
{
    unsigned long v = 0;
    if (clock.get_us)
    {
        v = clock.get_us ();
    }
    return v;
}

