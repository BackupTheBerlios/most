/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dev/arch/cpu.h"

extern void
DEV_cpudelay (unsigned long loop)
{
    while (--loop);
}
