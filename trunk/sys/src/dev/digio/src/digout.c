/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/heap.h>

#include "dev/digout.h"

extern void
DEV_digout_init (DEV_digout_t * out,
                 enum DEV_digio_state state,
                 enum DEV_digio_logig logig,
                 void (*set) (void), void (*clear) (void))
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
