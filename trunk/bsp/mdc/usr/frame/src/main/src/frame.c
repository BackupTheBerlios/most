/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>

#include "init/main.h"

#define VERSION "Frame 0.1.0"

extern void
MDC_main (void)
{
    USO_kputs (USO_LL_INFO, VERSION"\n");
}
