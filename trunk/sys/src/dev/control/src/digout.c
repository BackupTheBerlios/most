/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <uso/heap.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include <dev/digout.h>
#include <dev/cpu.h>

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    DEV_digout_t *out = (DEV_digout_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "state";
            entry->value.s = out->state == DEV_DIGIO_HIGH ? "1" : "0";
            break;
        case 1:
            entry->type = MFS_INFO_STRING;
            entry->name = "logic";
            entry->value.s = out->logig == DEV_DIGIO_POS ? "+" : "-";
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op digout_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};

extern void
DEV_digout_init (DEV_digout_t * out,
                 enum DEV_digio_state state,
                 enum DEV_digio_logig logig, void (*set) (void), void (*clear) (void))
{
    out->state = state;
    out->logig = logig;
    if (logig == DEV_DIGIO_POS)
    {
        out->set = set;
        out->clear = clear;
    }
    else
    {
        out->set = clear;
        out->clear = set;
    }
    if (state == DEV_DIGIO_HIGH)
    {
        out->set ();
    }
    else
    {
        out->clear ();
    }
}

extern void
DEV_digout_install (DEV_digout_t * out, char *name)
{
    MFS_descriptor_t * dir = MFS_resolve("/sys/dev/control/out");
    MFS_descriptor_create (dir, name, MFS_SYS, &digout_descriptor_op, (MFS_represent_t *) out);
    MFS_close_desc(dir);
}

extern void
DEV_digout_set (DEV_digout_t * out)
{
    out->state = DEV_DIGIO_HIGH;
    out->set ();
}

extern void
DEV_digout_clear (DEV_digout_t * out)
{
    out->state = DEV_DIGIO_LOW;
    out->clear ();
}

extern void
DEV_digout_toggle (DEV_digout_t * out)
{
    if (out->state == DEV_DIGIO_HIGH)
    {
        DEV_digout_clear (out);
    }
    else
    {
        DEV_digout_set (out);
    }
}

extern void
DEV_blink_nb (DEV_digout_t * out, unsigned long count, unsigned long time_ms)
{
    for (int c = 2 * count; c; --c)
    {
        DEV_digout_toggle (out);
        DEV_cpudelay (DEV_USEC_2_LOOPS (time_ms * (1000 / 2)));
    }
}

