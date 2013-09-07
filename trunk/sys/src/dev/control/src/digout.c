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
info (MFS_descriptor_t * desc)
{
    DEV_digout_t *out = (DEV_digout_t *) desc->represent;
    ACE_printf ("OUT state: %s , logig: %s\n",
                out->state == DEV_DIGIO_HIGH ? "1" : "0", out->logig == DEV_DIGIO_POS ? "+" : "-");
}

static struct MFS_descriptor_op digout_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
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
    MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/dev/control/out"), name,
                     MFS_SYS, &digout_descriptor_op, (MFS_represent_t *) out);
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

