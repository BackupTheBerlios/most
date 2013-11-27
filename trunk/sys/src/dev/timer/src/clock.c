/*
 *  Copyright (C) 2004-2007  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <ace/err.h>
#include <uso/sleep.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include <dev/clock.h>

static struct DEV_clock
{
    unsigned long ticks;
    unsigned long (*get_us) (void);
} clock;

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    switch (number){
        case 0:
            entry->type = MFS_INFO_LONG;
            entry->name = "Hz";
            entry->value.l = USO_MSEC_2_TICKS (1000);
            break;
        case 1:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Ticks";
            entry->value.z = clock.ticks;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "Usec";
            entry->value.z = DEV_get_usec ();
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op clock_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};


extern void
DEV_clock_init (unsigned long (*get_usec) (void))
{
    clock.ticks = 0;
    clock.get_us = get_usec;
    MFS_descriptor_t *dir = MFS_resolve("/sys/dev/timer");
    MFS_descriptor_create (dir, "clock",
                     MFS_SYS, &clock_descriptor_op, (MFS_represent_t *) &clock);
    MFS_close_desc(dir);
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

