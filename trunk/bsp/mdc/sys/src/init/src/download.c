/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <uso/cpu.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <cli/exec.h>
#include <dev/chips/flash_29F040.h>
#include <arch/reset.h>
#include <mfs/sysfs.h>
#include <mfs/block.h>
#include <init/config.h>
#include <init/download.h>

/* Sector 0: putboot, Sector 1: boot, Sector 2-7: app, end: 8 */
#define FLASH_START      0

static unsigned long sector_size;
static unsigned long sector_start;
static unsigned long sector_end;

static CLI_exec_t erase_flash;


struct flash {
	unsigned char *addr;
	unsigned long prog_size;
};

typedef struct flash flash_t;

static flash_t flash;

static ACE_err_t
flash_open (MFS_descriptor_t *desc)
{
	flash_t *fl = (flash_t *) desc->represent;
	MFS_block_t *bl = (MFS_block_t*)desc;
	if (desc->open_cnt != 1) return DEF_ERR_BUSY;
	fl->prog_size = 0;
    fl->addr = (unsigned char *)(bl->start * bl->size);
    return ACE_OK;
}

static void
flash_close (MFS_descriptor_t *desc)
{
	flash_t *fl = (flash_t *) desc->represent;
	MFS_block_t *bl = (MFS_block_t*)desc;
    fl->addr = (unsigned char *)(bl->start * bl->size);
}

static void
flash_info (MFS_descriptor_t *desc)
{
	flash_t *fl = (flash_t *) desc->represent;
	MFS_block_t *bl = (MFS_block_t*)desc;
    MFS_block_print(bl);
	ACE_printf("Flash ROM: prog_size = %lu, start = %p, end = %p \n", fl->prog_size, fl->addr, bl->end * bl->size);
}

static struct MFS_descriptor_op flash_desc_op = {
	.open = flash_open,
    .close = flash_close,
    .info = flash_info,
    .control = NULL,
};

static ACE_err_t
flash_put (MFS_descriptor_t * desc, char *buf, ACE_size_t len, ACE_size_t number)
{
	flash_t *fl = (flash_t *) desc->represent;
	MFS_block_t *bl = (MFS_block_t*)desc;
    enum FLASH_29F040_err_code error;

	if ( (fl->addr + len) >= (unsigned char *)(bl->end * bl->size) ) {
		return DEF_ERR_ROM;
	}

    error = FLASH_29F040_programm_s (fl->addr, (unsigned char *)buf, len);
    if (error)
    {
    	return DEF_ERR_ROM;
    }
    fl->addr += len;
    fl->prog_size += len;
    return ACE_OK;
}


static struct MFS_block_op flash_block_op = {
    .get = NULL,
    .put = flash_put,
    .confirm = NULL
};


static void
flash_init (flash_t * fl, MFS_descriptor_t * dir)
{
	MFS_descriptor_t *desc = MFS_block_create (dir, "flash", &flash_desc_op, &flash_block_op, (MFS_represent_t *) fl, MFS_BLOCK_IO);
	MFS_block_t *block = (MFS_block_t *)desc;
	block->start = sector_start;
	block->end = sector_end;
	block->size = sector_size;
	fl->prog_size = 0;
    fl->addr = (unsigned char *)(block->start * block->size);
}


static void
erase_flash_exec (char *nix)
{
    for (int i = sector_start; i < sector_end; ++i)
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
MDC_download_install (MFS_descriptor_t * dir, unsigned long start, unsigned long end)
{
	sector_start = start;
	sector_end = end;
    sector_size = FLASH_29F040_segment_size ();
	flash_init(&flash, dir);
    CLI_exec_init (dir, &erase_flash, "efl", "erase flash", erase_flash_exec);
}
