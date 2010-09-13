/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>

#include "arch/digio.h"
#include "boot.h"
#include "init/boot.h"
#include "download_app.h"
#include "download_boot.h"

extern void
MDC_main (void)
{
    USO_kputs (USO_LL_INFO, MDC_APPLICATION" "ACE_MOST_VERSION"\n");
    if (DEV_digin_isset(&MDC_switch) == FALSE){
    	MDC_start_app();
    }
    DEV_digout_set (&MDC_red_led);
	MDC_download_app_install();
	MDC_download_boot_install();
}
