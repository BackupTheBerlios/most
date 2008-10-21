/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>

#include "arch/digio.h"
#include "init/main.h"
#include "init/boot.h"
#include "download.h"

#define VERSION "PutBoot 0.1.0"

extern void
MDC_main (void)
{
    DEV_digout_set (&MDC_red_led);
    USO_kputs (USO_LL_INFO, VERSION"\n");
    if (DEV_digin_isset(&MDC_switch) == FALSE)
    	MDC_start_app();
    if (DEV_digin_isset(&MDC_button) == FALSE)
    	MDC_start_boot();
	MDC_download_install();
}
