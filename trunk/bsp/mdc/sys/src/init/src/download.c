/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <ace/stddef.h>
#include <uso/cpu.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <cli/exec.h>
#include <dev/chips/flash_29F040.h>
#include <arch/reset.h>
#include <mfs/sysfs.h>
#include <mfs/stream.h>
#include <init/config.h>
#include <init/download.h>



static CLI_exec_t erase_flash;


struct flash {
    unsigned long sector_size;
    unsigned char *sector_start;
    unsigned char *sector_end;
    ACE_bool_t erased;
};

typedef struct flash flash_t;

static flash_t flash;

static ACE_err_t
flash_open (MFS_descriptor_t *desc)
{
    MFS_stream_t *stream = (MFS_stream_t*)desc;
    if (desc->open_cnt == 0){
        stream->pos_rx = 0;
    }
    return ACE_OK;
}

static void
flash_info (MFS_descriptor_t *desc, int number, MFS_info_entry_t *entry)
{
    flash_t *fl = (flash_t *) desc->represent;
    switch (number){
        case 0:
        case 1:
        case 2:
            MFS_stream_info((MFS_stream_t *)desc, number, entry);
            break;
        case 3:
            entry->type = MFS_INFO_PTR;
            entry->name = "Loc";
            entry->value.p = MDC_FLASH_START;
            break;
        case 4:
            entry->type = MFS_INFO_PTR;
            entry->name = "Start";
            entry->value.p = fl->sector_start;
            break;
        case 5:
            entry->type = MFS_INFO_PTR;
            entry->name = "End";
            entry->value.p = fl->sector_end;
            break;
        case 6:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Max";
            entry->value.z = fl->sector_end - fl->sector_start;
            break;
        case 7:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Block";
            entry->value.z = fl->sector_size;
            break;
        case 8:
            entry->type = MFS_INFO_STRING;
            entry->name = "State";
            entry->value.s = (fl->erased == TRUE) ? "erased" : "written";
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
   }
}

static struct MFS_descriptor_op flash_desc_op = {
                .open = flash_open,
                .close = NULL,
                .info = flash_info,
                .control = NULL,
                .delete = NULL
};

static ACE_size_t
flash_write (MFS_stream_t *stream, const char *buf, ACE_size_t len)
{
    enum FLASH_29F040_err_code error;
    ACE_size_t l;
    unsigned char *p;
    flash_t *fl = (flash_t *) ((MFS_descriptor_t *)stream)->represent;

    if (stream->pos_rx == 0) {
        if (fl->erased == FALSE) return 0;
        fl->erased = FALSE;
        stream->size_tx = 0;
    }

    p =	(fl->sector_start + stream->pos_rx);

    if ( p >= fl->sector_end) {
        return ACE_EOF;
    }

    if ( (p + len) > fl->sector_end) {
        l = fl->sector_end - p;
    } else {
        l = len;
    }

    error = FLASH_29F040_programm_s (p, (unsigned char *)buf, len);
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
flash_init (flash_t *fl, unsigned int start, unsigned int end)
{
    fl->sector_size = FLASH_29F040_segment_size ();
    fl->sector_start = (unsigned char*)(start * fl->sector_size);
    fl->sector_end = (unsigned char*)(end * fl->sector_size);
    fl->erased = FALSE;
}


static ACE_err_t
erase_flash_exec (char *nix)
{
    for (unsigned char *addr = flash.sector_start; addr < flash.sector_end; addr += flash.sector_size)
    {
        enum FLASH_29F040_err_code error;
        error = FLASH_29F040_sector_erase_s (addr);
        ACE_printf ("Erase flash sector %p error %d\n", addr, error);
        if (error)
        {
            return DEF_ERR_ROM;
        }
    }
    flash.erased = TRUE;
    return ACE_OK;
}

extern void
MDC_download_install (MFS_descriptor_t * dir, unsigned int start, unsigned int end)
{
    flash_init(&flash, start, end);
    CLI_exec_init (dir, &erase_flash, "efl", "erase flash", erase_flash_exec);
    MFS_stream_create (dir, "flash", &flash_desc_op, &flash_stream_op, (MFS_represent_t *) &flash, MFS_FILE);
}
