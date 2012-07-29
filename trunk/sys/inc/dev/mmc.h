#ifndef DEV_MMC_H
#define DEV_MMC_H

#include <ace/stdlib.h>
#include <dev/spi.h>
#include <dev/err.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup mmc mmc.h
 *
 * @{
 */

/**
 * MMC Block Size
 */
#define DEV_MMC_BLOCK_SIZE     512

struct DEV_mmc_CID
{
    unsigned char mid;
    char name[8];
    ACE_u32_t serial_nr;
};

struct DEV_mmc_CSD
{
    unsigned int read_block_len;
    ACE_bool_t read_block_partial;
    unsigned int write_block_len;
    ACE_bool_t write_block_partial;
    unsigned long block_nr;     /* nummber of blocks */
    unsigned long trans_speed;  /* kHz */
};

/** Initialize SPI communication to MMC Card.
 * @return error code
 */
extern ACE_err_t DEV_mmc_spi_init (DEV_spi_dev_t * dev);

/** Initialize MMC Card.
 * @return error code
 */
extern ACE_err_t DEV_mmc_init (void);

/**
 * Install mmc in sysfs.
 */
extern void DEV_mmc_install (void);

/** read a size Byte big block beginning at the address.
 * @param address : address of datablock
 * @param count : count of data to read
 * @param buffer : buffer with free space for data
 * @return error code
 */
extern ACE_err_t DEV_mmc_read_block (const unsigned long address, const unsigned long count,
                                     char buffer[]);

/** write a 512 Byte big block beginning at the (aligned) address
 * @param address : address of datablock
 * @param buffer : buffer containing the block data
 * @return Error code
 */
extern ACE_err_t DEV_mmc_write_block (const unsigned long address, char buffer[]);

/** Reading the contents of the CID.
 * @param cid : structure where CID data is stored
 * @return error code
 */
extern ACE_err_t DEV_mmc_read_CID (struct DEV_mmc_CID *cid);

/** Reading the contents of the CSD.
 * @param cid : structure where CSD data is stored
 * @return error code
 */
extern ACE_err_t DEV_mmc_read_CSD (struct DEV_mmc_CSD *csd);


/** @}
 */

/** @}
 */

#endif
