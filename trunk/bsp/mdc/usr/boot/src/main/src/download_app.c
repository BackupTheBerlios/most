/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <uso/arch/cpu.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <cli/commands.h>
#include <dev/chips/flash_29F040.h>
#include <arch/reset.h>
#include <mfs/sysfs.h>
#include <init/config.h>
#include <download_app.h>

/* Sector 0: save, Sector 1: boot Sector 2-7: app */
#define APP_SECTOR_START 2
#define APP_SECTOR_END   7

static CLI_exec_t eth_download_app;
static CLI_exec_t serial_download_app;
static CLI_exec_t erase_flash_app;

static unsigned long sector_size;
static unsigned char *addr;
static unsigned long prog_size;

static ACE_bool_t
prog_data (char *data, ACE_size_t len)
{
    enum FLASH_29F040_err_code error;
    error = FLASH_29F040_programm_s (addr, (unsigned char *)data, len);
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
        addr = (unsigned char *)(sector_size * APP_SECTOR_START);
        if (NAP_tftp_open (&MDC_config.ip_addr, &MDC_config.server) >= 0)
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

static void
serial_download_app_exec (char *file_size)
{
    if (file_size)
    {
        char *state = "done";
        unsigned long app_size = ACE_atol (file_size);
        prog_size = 0;
        addr = (unsigned char *)(sector_size * APP_SECTOR_START);
        ACE_printf ("Send file len %ld\n", app_size);
        unsigned long data;
        while ((prog_size + sizeof (data)) <= app_size)
        {
            ACE_read ((char *)&data, sizeof (data));
            if (prog_data ((char *)&data, sizeof (data)) == FALSE)
            {
                app_size = prog_size;
                state = "fail";
                break;
            }
        }
        ACE_size_t left_over = app_size - prog_size;
        if (left_over)
        {
            ACE_read ((char *)(&data), left_over);
            if (prog_data ((char *)&data, left_over) == FALSE)
            {
                state = "fail";
            }
        }
        ACE_printf ("Download %s %ld\n", state, prog_size);
    }
    else
    {
        ACE_puts ("Give boot size as param\n");
    }
}

static void
erase_flash_app_exec (char *nix)
{
    for (int i = APP_SECTOR_START; i <= APP_SECTOR_END; ++i)
    {
        enum FLASH_29F040_err_code error;
        error = FLASH_29F040_sector_erase_s ((unsigned char *)(i * sector_size));
        ACE_printf ("Erase flash sector %d error %d\n", i, error);
        if (error)
        {
            break;
        }
    }
}

extern void
MDC_download_app_install (MFS_descriptor_t * boot)
{
    sector_size = FLASH_29F040_segment_size ();
    CLI_exec_init (boot, &eth_download_app, "edla", "Eth download app", eth_download_app_exec);
    CLI_exec_init (boot, &serial_download_app, "sdla", "Serial download app",
                   serial_download_app_exec);
    CLI_exec_init (boot, &erase_flash_app, "efla", "Erase flash app", erase_flash_app_exec);
}
