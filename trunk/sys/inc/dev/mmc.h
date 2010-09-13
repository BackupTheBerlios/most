#ifndef _DEV_MMC_H_
#define _DEV_MMC_H_

#include "dev/spi.h"

// error/success codes
#define MMC_SUCCESS           0x00
#define MMC_BLOCK_SET_ERROR   0x01
#define MMC_RESPONSE_ERROR    0x02
#define MMC_DATA_TOKEN_ERROR  0x03
#define MMC_INIT_ERROR        0x04
#define MMC_CRC_ERROR         0x10
#define MMC_WRITE_ERROR       0x11
#define MMC_OTHER_ERROR       0x12
#define MMC_TIMEOUT_ERROR     0xFF

extern char mmc_buffer[512];

extern int DEV_mmc_spi_init(DEV_spi_dev_t *dev);

extern char DEV_mmc_init(void);

// Reading the contents of the CSD and CID registers in SPI mode is a simple
// read-block transaction.
extern char DEV_mmc_read_register (const char cmd_register, const unsigned char length);

// read a size Byte big block beginning at the address.
extern char DEV_mmc_read_block(const unsigned long address, const unsigned long count);

// write a 512 Byte big block beginning at the (aligned) address
extern char DEV_mmc_write_block (const unsigned long address);


#endif
