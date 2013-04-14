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
#include <mfs/stream.h>
#include <init/download.h>

struct flash {
	unsigned long sector_size;
	unsigned char *sector_start;
	unsigned char *sector_end;
};

typedef struct flash flash_t;

static flash_t flash;

static ACE_err_t
flash_open (MFS_descriptor_t *desc)
{
	MFS_stream_t *stream = (MFS_stream_t *) desc;
	if (desc->open_cnt != 1) return DEF_ERR_BUSY;
	stream->pos_rx = 0;
    return ACE_OK;
}

static void
flash_close (MFS_descriptor_t *desc)
{
}

static void
flash_info (MFS_descriptor_t *desc)
{
	flash_t *fl = (flash_t *) desc->represent;
    MFS_stream_print((MFS_stream_t *)desc);
	ACE_printf("\t Flash: loc = %p,  start = %p, end = %p, max = %lu, block size = %lu\n",
			SAM_FLASH_START, fl->sector_start, fl->sector_end, fl->sector_end - fl->sector_start, fl->sector_size);
}

static struct MFS_descriptor_op flash_desc_op = {
	.open = flash_open,
    .close = flash_close,
    .info = flash_info,
    .control = NULL,
};

static ACE_size_t
flash_write (MFS_stream_t * stream, const char *buf, ACE_size_t len)
{
    unsigned char error;
    ACE_size_t l;
    unsigned char *p;
	flash_t *fl = (flash_t *) ((MFS_descriptor_t *)stream)->represent;

    if (stream->pos_rx == 0) stream->size_tx = 0;

	p =	(fl->sector_start + stream->pos_rx);

	if ( p >= fl->sector_end) {
		return ACE_EOF;
	}

	if ( (p + len) > fl->sector_end) {
		l = fl->sector_end - p;
	} else {
		l = len;
	}

    error = DEV_at91_FLASHD_write ((unsigned int)p, (unsigned char *)buf, (unsigned int)l);
    if (error)
    {
		return 0; /* todo return error value */
    }

    stream->pos_rx += l;
    stream->size_tx += l;
    return l;
}


static struct MFS_stream_op flash_stream_op = {
    .read = NULL,
    .write = flash_write,
    .seek = NULL,
    .flush = NULL
};


static void
flash_init (flash_t *fl, unsigned char *start, unsigned char *end)
{
	fl->sector_size = AT91C_IFLASH_PAGE_SIZE;
	fl->sector_start = start;
	fl->sector_end = end;
}


extern void
SAM_download_install (MFS_descriptor_t * dir, unsigned char *start, unsigned char *end)
{
	flash_init(&flash, start, end);
	MFS_stream_create (dir, "flash", &flash_desc_op, &flash_stream_op, (MFS_represent_t *) &flash, MFS_FILE);
}

