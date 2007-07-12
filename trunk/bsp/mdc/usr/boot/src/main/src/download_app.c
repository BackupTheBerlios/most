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
#include <arch/29F040.h>
#include <arch/reset.h>

#include "init/boot.h"
#include "download_app.h"

/* Sector 0: save, Sector 1: boot Sector 2-7: app */
#define APP_SECTOR_START 2
#define APP_SECTOR_END   7

static CLI_exec_t eth_download_app;
static CLI_exec_t serial_download_app;
static CLI_exec_t erase_flash_app;

static unsigned long sector_size;
static unsigned char *addr;
static unsigned long prog_size; 

static bool_t
prog_data(char* data, size_t len)
{
	enum FLASH_29F040_err_code error;
	error =  FLASH_29F040_programm_s (addr, (unsigned char*)data, len);
	if (error){
		return FALSE;
	}
	addr += len;
	prog_size += len;
	return TRUE;
}

static void
eth_download_app_exec (char *file)
{
	if (file){
		prog_size = 0;
		addr = (unsigned char*)(sector_size * APP_SECTOR_START);
		NAP_tftp_open(NAP_bootp_ip_address(), NAP_bootp_server_address());
		NAP_tftp_get(file, prog_data);
		//NAP_tftp_close();
		printf("Download done %ld\n", prog_size);
	} else {
		puts("Give file name as param\n");
	}
}

static void
serial_download_app_exec(char* file_size)
{
	if (file_size){
		char *state = "done";
		unsigned long app_size = atol(file_size);
		prog_size = 0;
		addr = (unsigned char*)(sector_size * APP_SECTOR_START);
		printf("Send file len %ld\n", app_size);
		unsigned long data;
		while ( (prog_size + sizeof(data)) <= app_size){
			read((char*)&data, sizeof(data));
			if (prog_data((char*)&data, sizeof(data)) == FALSE){
				app_size = prog_size;
				state = "fail";
				break;
			}
		}
		size_t left_over = app_size - prog_size;
		if (left_over){
			read((char*)(&data), left_over);
			if (prog_data((char*)&data, left_over) == FALSE){
				state = "fail";
			}
		}
		printf("Download %s %ld\n", state, prog_size);
	} else {
		puts("Give boot size as param\n");
	}
}

static void
erase_flash_app_exec (char *nix)
{
	for (int i = APP_SECTOR_START; i <= APP_SECTOR_END; ++i){
		enum FLASH_29F040_err_code error;
		error =	FLASH_29F040_sector_erase_s ((unsigned char*)(i * sector_size));
		printf("Erase flash sector %d error %d\n", i, error);
		if (error){
			break;
		}
	}
}

extern void
MDC_download_app_install (void)
{
	sector_size = FLASH_29F040_segment_size();
    CLI_exec_init (boot, &eth_download_app, "edlda", "Eth download app", eth_download_app_exec);
    CLI_exec_init (boot, &serial_download_app, "sdlda", "Serial download app", serial_download_app_exec);
    CLI_exec_init (boot, &erase_flash_app, "efla", "Erase flash app", erase_flash_app_exec);
}
