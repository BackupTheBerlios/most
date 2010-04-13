/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dev/arch/cpu.h"

extern void
DEV_cpudelay (unsigned long loop)
{
  volatile unsigned long i = loop; /* because compiler optimization */
  while (i){
    --i;
  }
}
