/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>

#include "frame.h"

extern void
MDC_main (void)
{
    USO_kputs (USO_LL_INFO, MDC_APPLICATION" "ACE_MOST_VERSION"\n");
}
