/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/heap.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include "dev/digout.h"
#include "dev/cpu.h"

static void info (MFS_entry_t * entry);

static struct MFS_descriptor_op digout_descriptor_op = {.open = NULL,
    .close = NULL,
    .info = info
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
    MFS_create_desc (MFS_sysfs_get_dir (MFS_SYSFS_DIR_DIGIO), name,
                     (MFS_entry_t *) out, MFS_DESC, &digout_descriptor_op);
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

static void
info (MFS_entry_t * entry)
{
    DEV_digout_t *out = (DEV_digout_t *) entry;
    ACE_printf ("OUT state: %s , logig: %s\n",
                out->state == DEV_DIGIO_HIGH ? "1" : "0", out->logig == DEV_DIGIO_POS ? "+" : "-");
}
