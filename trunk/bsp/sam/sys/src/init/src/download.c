/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 * use USO_log or DEBUGF only if it doesn't write to stdout, otherwise it will break the ymodem connection !
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <uso/cpu.h>
#include <dev/arch/at91/flashd.h>
#include <dev/arch/at91/efc.h>
#include <mfs/block.h>
#include <init/download.h>

#define PAGE_SIZE    AT91C_IFLASH_PAGE_SIZE

#define FLASH_START        (unsigned char *)(0x100000)

static unsigned char *sector_start;
static unsigned char *sector_end;

struct flash {
	unsigned char *addr;
	unsigned long prog_size;
};

typedef struct flash flash_t;

static flash_t flash;

static ACE_err_t
flash_open (MFS_descriptor_t *block)
{
	flash_t *fl = (flash_t *) block->represent;
	if (block->open_cnt != 1) return DEF_ERR_BUSY;
	fl->prog_size = 0;
    fl->addr = sector_start;
    return ACE_OK;
}

static void
flash_close (MFS_descriptor_t *block)
{
	flash_t *fl = (flash_t *) block->represent;
    fl->addr = sector_start;
}

static void
flash_info (MFS_descriptor_t *block)
{
	flash_t *fl = (flash_t *) block->represent;
    MFS_block_print((MFS_block_t *)block);
	ACE_printf("Flash ROM: prog_size = %lu, start = %p, end = %p \n", fl->prog_size, fl->addr, sector_end);
}

static struct MFS_descriptor_op flash_desc_op = {
	.open = flash_open,
    .close = flash_close,
    .info = flash_info,
    .control = NULL,
};

static ACE_err_t
flash_put (MFS_descriptor_t * block, char *buf, ACE_size_t len, ACE_size_t number)
{
	flash_t *fl = (flash_t *) block->represent;
    unsigned char error;

	if ( (fl->addr + len) >= sector_end) {
		return DEF_ERR_ROM;
	}

    error = DEV_at91_FLASHD_write ((unsigned int)fl->addr, (unsigned char *)buf, (unsigned int)len);
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
	block->start = (sector_start - FLASH_START) / PAGE_SIZE;
	block->end = (sector_end - FLASH_START) / PAGE_SIZE;
	block->size = PAGE_SIZE;
	fl->prog_size = 0;
    fl->addr = (unsigned char *)(sector_start);
}


extern void
SAM_download_install (MFS_descriptor_t * dir, unsigned char *start, unsigned char *end)
{
	sector_start = start;
	sector_end = end;
	flash_init(&flash, dir);
}

