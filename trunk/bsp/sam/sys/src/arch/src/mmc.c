/*
 * mmc.c
 *
 *  Created on: 10.02.2013
 *      Author: maik
 */

#include <ace/assert.h>
#include <uso/log.h>
#include <uso/buf_pool.h>
#include <dev/mmc.h>
#include <mfs/block.h>
#include <arch/spi.h>
#include <arch/digio.h>
#include <arch/mmc.h>
#include <init/err.h>

#define SAM_GET_BUFS              1
#define SAM_MMC_BLOCK_SIZE        (DEV_MMC_BLOCK_SIZE)
#define SAM_PARTITION_1_BLOCKS    ((unsigned long)10)
#define SAM_PARTITION_2_BLOCKS    ((unsigned long)10)
#define SAM_PARTITION_3_BLOCKS    ((unsigned long)(512 * 8 + 2))

#define SAM_PARTITION_1_START     ((unsigned long)0)
#define SAM_PARTITION_1_END       (SAM_PARTITION_1_START + SAM_PARTITION_1_BLOCKS)
#define SAM_PARTITION_2_START     (SAM_PARTITION_1_END)
#define SAM_PARTITION_2_END       (SAM_PARTITION_2_START + SAM_PARTITION_2_BLOCKS)
#define SAM_PARTITION_3_START     (SAM_PARTITION_2_END)
#define SAM_PARTITION_3_END       (SAM_PARTITION_3_START + SAM_PARTITION_3_BLOCKS)


struct disc {
    char *buf;
};

typedef struct disc disc_t;

static disc_t d1_p1;
static disc_t d1_p2;
static disc_t d1_p3;

USO_buf_pool_t get_pool;
char get_bufs[SAM_GET_BUFS][DEV_MMC_BLOCK_SIZE];

static ACE_err_t
mmc_open (MFS_descriptor_t *desc)
{
    if (desc->open_cnt != 1) return DEF_ERR_BUSY;
    return ACE_OK;
}

static void
mmc_close (MFS_descriptor_t *desc)
{
}

static void
mmc_info (MFS_descriptor_t *desc)
{
    MFS_block_print((MFS_block_t *)desc);
}

static struct MFS_descriptor_op mmc_desc_op ={
    .open = mmc_open,
    .close = mmc_close,
    .info = mmc_info,
    .control = NULL,
};

static ACE_err_t
mmc_put (MFS_block_t * block, char *buf, ACE_size_t len, ACE_size_t number)
{
    return DEV_mmc_write_block (number * block->size, buf);
}

static ACE_ssize_t
mmc_get (MFS_block_t *block, char **buf, ACE_size_t number)
{
    disc_t *disc = (disc_t *) ((MFS_descriptor_t *)block)->represent;
    ACE_err_t ret;
    disc->buf = USO_buf_alloc (&get_pool);
    ret = DEV_mmc_read_block (number * block->size, block->size, disc->buf);
    if (ret != ACE_OK) {
        USO_buf_free (&get_pool, disc->buf);
        *buf = NULL;
        return ret;
    }
    *buf = disc->buf;
    return block->size;
}

static ACE_err_t
mmc_confirm (MFS_block_t *block, ACE_size_t number)
{
    disc_t *disc = (disc_t *) ((MFS_descriptor_t *)block)->represent;
    if (disc->buf != NULL) {
        USO_buf_free (&get_pool, disc->buf);
        disc->buf = NULL;
    } else {
        return DEF_ERR_NULL;
    }
    return ACE_OK;
}

static struct MFS_block_op mmc_block_op = {
    .get = mmc_get,
    .put = mmc_put,
    .confirm = mmc_confirm
};


static void
mmc_init (MFS_descriptor_t *desc, unsigned long start, unsigned long end, ACE_size_t size)
{
    disc_t *disc = (disc_t *) desc->represent;
    MFS_block_t *block = (MFS_block_t *)desc;
    block->start = start;
    block->end = end;
    block->size = size;
    disc->buf = NULL;
}

static void
disc_install(MFS_descriptor_t *dir)
{
    MFS_descriptor_t *mmc;
    mmc = MFS_directory_create (dir, "mmc");

    MFS_descriptor_t *desc = MFS_block_create (mmc, "cfg", &mmc_desc_op, &mmc_block_op, (MFS_represent_t *) &d1_p1, MFS_DISK);
    mmc_init(desc, SAM_PARTITION_1_START, SAM_PARTITION_1_END, SAM_MMC_BLOCK_SIZE);

    desc = MFS_block_create (mmc, "tst", &mmc_desc_op, &mmc_block_op, (MFS_represent_t *) &d1_p2, MFS_DISK);
    mmc_init(desc, SAM_PARTITION_2_START, SAM_PARTITION_2_END, SAM_MMC_BLOCK_SIZE);

    desc = MFS_block_create (mmc, "sfs", &mmc_desc_op, &mmc_block_op, (MFS_represent_t *) &d1_p3, MFS_DISK);
    mmc_init(desc, SAM_PARTITION_3_START, SAM_PARTITION_3_END, SAM_MMC_BLOCK_SIZE);

    USO_buf_pool_init (&get_pool, get_bufs, SAM_GET_BUFS, SAM_MMC_BLOCK_SIZE);
}

extern ACE_err_t
SAM_mmc_install(void)
{
    ACE_err_t ret;
    if ( (ret = DEV_mmc_spi_init ((DEV_spi_dev_t *) &SAM_mmc)) < ACE_OK)
    {
        DEV_digout_set (&SAM_red_led);
        USO_log_puts (USO_LL_ERROR, "MMC spi init failed.\n");
        return ret;
    }
    else if ( (ret = DEV_mmc_init ()) < ACE_OK)
    {
        DEV_digout_set (&SAM_red_led);
        USO_log_puts (USO_LL_ERROR, "MMC card not found.\n");
        return ret;
    }
    DEV_mmc_install ();
    disc_install(MFS_resolve(MFS_get_root(), "bsp"));
    return ACE_OK;
}
