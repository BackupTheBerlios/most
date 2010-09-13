/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>

#include "arch/digio.h"
#include "putboot.h"
#include "init/boot.h"
#include "download.h"

extern void
MDC_main (void)
{
    USO_kputs (USO_LL_INFO, MDC_APPLICATION" "ACE_MOST_VERSION"\n");
    if (DEV_digin_isset(&MDC_switch) == FALSE)
    	MDC_start_app();
    if (DEV_digin_isset(&MDC_button) == FALSE)
    	MDC_start_boot();
    DEV_digout_set (&MDC_red_led);
	MDC_download_install();
}
