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
    ACE_printf("\t Flash:\n"
               "\t loc = %p,\n"
               "\t start = %p,\n"
               "\t end = %p,\n"
               "\t max = %lu,\n"
               "\t block size = %lu,\n"
               "\t flash %s\n",
                    LA2_FLASH_START,
                    fl->sector_start,
                    fl->sector_end,
                    fl->sector_end - fl->sector_start,
                    fl->sector_size,
                    fl->erased == TRUE ? "erased" : "written");
}

static struct MFS_descriptor_op flash_desc_op = {
    .open = flash_open,
    .close = flash_close,
    .info = flash_info,
    .control = NULL,
};

static ACE_size_t
flash_read (MFS_stream_t *stream, char *buf, ACE_size_t len)
{
    flash_t *fl = (flash_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t read;
    if (fl->erased == TRUE) {
        stream->size_tx = 0;
        return 0; /* todo EOF ? */
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
    enum FLASH_26LV800BTC_err_code error;
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

    error = FLASH_26LV800BTC_programm_is ((unsigned short *)p, buf, l);
    if (error)
    {
        return 0; /* todo return error value */
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


static void
erase_flash_exec (char *nix)
{
    int i = flash.sector_index; 
    for (unsigned char *addr = flash.sector_start; (addr < flash.sector_end) && (sector_size[i] > 0); addr += (sector_size[i] * 1024), ++i)
    {
        enum FLASH_26LV800BTC_err_code error;
        error = FLASH_26LV800BTC_sector_erase_is ((unsigned short *)addr);
        ACE_printf ("Erase flash sector %p error %d\n", addr, error);
        if (error)
        {
            return;
        }
    }
    flash.erased = TRUE;
}

static void
print_flash_id_exec (char *nix)
{
    unsigned short mf, device;
    enum FLASH_26LV800BTC_err_code error;
    error = FLASH_26LV800BTC_get_id_is (&mf, &device);
    ACE_printf ("Flash err = %d, mf id = %X, device id = %X\n", error, mf, device);
}

extern void
LA2_download_install (MFS_descriptor_t * dir, unsigned char *start, unsigned char *end)
{
    flash_init(&flash, start, end);
    CLI_exec_init (dir, &erase_flash, "efl", "erase flash", erase_flash_exec);
    CLI_exec_init (dir, &print_flash_id, "pflid", "print flash id", print_flash_id_exec);
    MFS_stream_create (dir, "flash", &flash_desc_op, &flash_stream_op, (MFS_represent_t *) &flash, MFS_FILE);
}
