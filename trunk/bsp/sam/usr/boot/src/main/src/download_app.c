/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <uso/arch/cpu.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <cli/commands.h>
#include <dev/arch/at91/flashd.h>
#include <dev/arch/at91/efc.h>
#include <arch/reset.h>

#include "init/config.h"
#include "download_app.h"

/* Bootloader: 64K 0x100000 - 0x110000, Application : 3*64K = 192K 0x110000 - 0x140000 */
#define APP_SECTOR_START 0x110000
#define APP_SECTOR_END   0x140000

static CLI_exec_t eth_download_app;

static unsigned char *addr;
static unsigned long prog_size;

static ACE_bool_t
prog_data (char *data, ACE_size_t len)
{
    unsigned char error;
    error = DEV_at91_FLASHD_write ((unsigned int)addr, (unsigned char *)data, len);
    if (error)
    {
        return FALSE;
    }
    addr += len;
    prog_size += len;
    return TRUE;
}

static void
eth_download_app_exec (char *file)
{
    if (file)
    {
        prog_size = 0;
        addr = (unsigned char *)(APP_SECTOR_START);
        if (NAP_tftp_open (&SAM_config.ip_addr, &SAM_config.server) >= 0)
        {
            if (NAP_tftp_get (file, prog_data) >= 0)
            {
                ACE_printf ("Download done %ld\n", prog_size);
            }
            else
            {
                ACE_puts ("Tftp get failed\n");
            }
            NAP_tftp_close ();
        }
        else
        {
            ACE_puts ("Tftp open failed\n");
        }
    }
    else
    {
        ACE_puts ("Give file name as param\n");
    }
}



extern void
SAM_download_app_install (MFS_descriptor_t * boot)
{
    CLI_exec_init (boot, &eth_download_app, "edla", "Eth download app", eth_download_app_exec);
}
