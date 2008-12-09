/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <uso/arch/cpu.h>
#include <cli/commands.h>
#include <arch/29F040.h>

#include "init/boot.h"

#include "download.h"

/* Sector 0: save, Sector 1: boot Sector 2-7: app */
#define BOOT_SECTOR 1

static CLI_exec_t serial_download_boot;
static CLI_exec_t erase_flash_boot;

static unsigned long sector_size;

static void
serial_download_boot_exec(char* file_size)
{
	if (file_size){
    	long len = ACE_atol(file_size);
    	char* buffer = ACE_malloc(len);
    	if (buffer){
    		unsigned char *addr;
            ACE_printf("Send file len %ld\n", len);
    		ACE_read(buffer, len);
    		enum FLASH_29F040_err_code error;
    		addr = (unsigned char*)(sector_size * BOOT_SECTOR);
    		error =  FLASH_29F040_programm_s (addr, (unsigned char*)buffer, len);
    		ACE_printf("Programm flash addr %p len %ld error %d\n", addr, len, error);
    		ACE_free(buffer);
    	} else {
    		ACE_printf("Not enough ram %ld\n", len);
    	}
	} else {
		ACE_puts("Give boot size as param\n");
	}
}

static void
erase_flash_boot_exec (char *nix)
{
	for (int i = BOOT_SECTOR; i <= BOOT_SECTOR; ++i){
		enum FLASH_29F040_err_code error;
		error =	FLASH_29F040_sector_erase_s ((unsigned char*)(i * sector_size));
		ACE_printf("Erase flash sector %d error %d\n", i, error);
		if (error){
			break;
		}
	}
}

extern void
MDC_download_install (void)
{
	sector_size = FLASH_29F040_segment_size();
    CLI_exec_init (boot, &serial_download_boot, "sdldb", "Serial download boot", serial_download_boot_exec);
    CLI_exec_init (boot, &erase_flash_boot, "eflb", "Erase flash boot", erase_flash_boot_exec);
}
