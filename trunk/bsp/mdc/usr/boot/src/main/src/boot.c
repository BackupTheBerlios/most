/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/log.h>

#include "init/main.h"
#include "download_app.h"
#include "download_boot.h"

#define VERSION "Boot 0.1.0"

extern void
MDC_main (void)
{
    USO_kputs (USO_LL_INFO, VERSION"\n");
	MDC_download_app_install();
	MDC_download_boot_install();
}
