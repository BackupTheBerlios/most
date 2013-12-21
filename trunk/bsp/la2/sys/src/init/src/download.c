/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <ace/stddef.h>
#include <uso/cpu.h>
#include <uso/scheduler.h>
#include <nap/bootp.h>
#include <nap/tftp.h>
#include <cli/exec.h>
#include <dev/chips/flash_26LV800BTC.h>
#include <arch/reset.h>
#include <mfs/sysfs.h>
#include <mfs/stream.h>
#include <init/config.h>
#include <init/download.h>



static CLI_exec_t erase_flash;
static CLI_exec_t print_flash_id;


struct flash {
    unsigned long sector_size;
    unsigned char *sector_start;
    unsigned char *sector_end;
    int sector_index;
    ACE_bool_t erased;
};

typedef struct flash flash_t;

static flash_t flash;

/* flash sector sizes in KB */
static int sector_size[20] = {16, 8, 8, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 0};

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
            entry->value.p = LA2_FLASH_START;
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
flash_read (MFS_stream_t *stream, char *buf, ACE_size_t len)
{
    flash_t *fl = (flash_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t read;
    if (fl->erased == TRUE) {
        stream->size_tx = 0;
        return 0;
    }
    /* if flash is written but size == 0(this is after a reboot, read the first 2 sectors */
    if (stream->size_tx == 0){
        stream->size_tx = 128 * 1024;
    }
    unsigned char *p;
    p = (fl->sector_start + stream->pos_rx);
    for (read = 0; (read < len) && (p < fl->sector_start + stream->size_tx); ++read, ++p, ++buf){
        *buf = *p;
    }
    stream->pos_rx += read;
    return read;
}

static ACE_size_t
flash_write (MFS_stream_t *stream, const char *buf, ACE_size_t len)
{
    ACE_err_t err;
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
        return 0;
    }

    if ( (p + len) > fl->sector_end) {
        l = fl->sector_end - p;
    } else {
        l = len;
    }

    err = FLASH_26LV800BTC_programm_is ((unsigned short *)p, buf, l);
    if (err < ACE_OK)
    {
        USO_current()->error = err;
        return 0;
    }
    stream->pos_rx += l;
    stream->size_tx += l;
    return l;
}


static struct MFS_stream_op flash_stream_op = {
    .read = flash_read,
    .write = flash_write,
    .seek = NULL,
    .flush = NULL
};



static void
flash_init (flash_t *fl, unsigned char *start, unsigned char *end)
{
    ACE_size_t addr;
    int i;
    fl->sector_size = FLASH_26LV800BTC_segment_size ();
    fl->sector_start = start;
    fl->sector_end = end;
    fl->erased = FALSE;
    addr = fl->sector_start - LA2_FLASH_START;
    for (i = 0; (addr > 0) && (addr < (LA2_BOOT_SIZE + LA2_APPL_SIZE)); ++i){
        addr -= (sector_size[i] * 1024);
    }
    fl->sector_index = i;
}


static ACE_err_t
erase_flash_exec (char *nix)
{
    ACE_err_t err = DEV_ERR;
    int i = flash.sector_index; 
    for (unsigned char *addr = flash.sector_start;
         (addr < flash.sector_end) && (sector_size[i] > 0);
         addr += (sector_size[i] * 1024), ++i)
    {
        err = FLASH_26LV800BTC_sector_erase_is ((unsigned short *)addr);
        ACE_printf ("Erase flash sector %p error %d\n", addr, err);
    }
    flash.erased = TRUE;
    return err;
}

static ACE_err_t
print_flash_id_exec (char *nix)
{
    unsigned short mf, device;
    ACE_err_t err;
    err = FLASH_26LV800BTC_get_id_is (&mf, &device);
    ACE_printf ("Flash err = %d, mf id = %X, device id = %X\n", err, mf, device);
    return err;
}

extern void
LA2_download_install (MFS_descriptor_t * dir, unsigned char *start, unsigned char *end)
{
    flash_init(&flash, start, end);
    CLI_exec_init (dir, &erase_flash, "efl", "erase flash", erase_flash_exec);
    CLI_exec_init (dir, &print_flash_id, "pflid", "print flash id", print_flash_id_exec);
    MFS_stream_create (dir, "flash", &flash_desc_op, &flash_stream_op, (MFS_represent_t *) &flash, MFS_FILE);
}
