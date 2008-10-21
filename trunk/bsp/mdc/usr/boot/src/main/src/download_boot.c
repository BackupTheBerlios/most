/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stddef.h>

#include <uso/arch/cpu.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <cli/commands.h>
#include <arch/29F040.h>
#include <arch/reset.h>

#include "init/boot.h"
#include "init/config.h"
#include "download_boot.h"

/* Sector 0: putboot, Sector 1: boot Sector 2-7: app */
#define BOOT_SECTOR      1

static CLI_exec_t eth_download_boot;
static CLI_exec_t serial_download_boot;
static CLI_exec_t prog_flash_boot;

static unsigned long sector_size;
static unsigned char *addr;
static unsigned char *boot_base;
static unsigned long boot_size; 

static bool_t
recv_data(char* data, size_t len)
{
	if (boot_size + len > sector_size){
		printf("Boot size > sector size\n");
		return FALSE;
	}
	memcpy(addr, data, len);
	addr += len;
	boot_size += len;
	return TRUE;
}

static void
eth_download_boot_exec (char *file)
{
	if (file){
		addr = malloc(sector_size);
		if (addr){
			boot_size = 0;
			boot_base = addr;
			if (NAP_tftp_open(&MDC_ee_config.ip_addr, &MDC_ee_config.server) >= 0){
				if (NAP_tftp_get(file, recv_data) >= 0){
					printf("Download done %lu\n", boot_size);
				} else {
					puts("Tftp get failed\n");
					free(addr);
				}
				NAP_tftp_close();
			} else {
				puts("Tftp open failed\n");
				free(addr);
			}
		} else {
			puts("Ram not available\n");
		}
	} else {
		puts("Give file name as param\n");
	}
}

static void
serial_download_boot_exec(char* file_size)
{
	if (file_size){
		boot_size = atol(file_size);
		if (boot_size <= sector_size){
			boot_base = malloc(boot_size);
			if (boot_base){
				printf("Send file len %ld\n", boot_size);
				read((char*)boot_base, boot_size);
				puts("Download done\n");
			} else {
				printf("Ram not available %ld\n", boot_size);
			}
		} else {
			printf("Boot size > sector size %ld\n", boot_size);
		}
	} else {
		puts("Give boot size as param\n");
	}
}

static void prog_flash(void) _SECTION_ (".ramcode");

static void prog_flash(void)
{
	enum FLASH_29F040_err_code error;
	error =	FLASH_29F040_sector_erase_ns (addr);
	if (error == FLASH_29F040_ok){
		error =  FLASH_29F040_programm_ns (addr, boot_base, boot_size);
		if (error == FLASH_29F040_ok){
			long i;
			for(i = 1; i < 100000; ++i);
			MDC_jump_boot();
		}
	}
	for(;;);
}


static void
prog_flash_boot_exec (char *nix)
{
    USO_disable ();
	addr = (unsigned char*)(BOOT_SECTOR * sector_size);
	prog_flash();
}

extern void
MDC_download_boot_install (void)
{
	sector_size = FLASH_29F040_segment_size();
    CLI_exec_init (boot, &eth_download_boot, "edlb", "Eth download boot", eth_download_boot_exec);
    CLI_exec_init (boot, &serial_download_boot, "sdlb", "Serial download boot", serial_download_boot_exec);
    CLI_exec_init (boot, &prog_flash_boot, "pflb", "Prog flash boot", prog_flash_boot_exec);
}
