/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 ** low level support for 93c46b 1 kbit serial eeprom
 * 128 Bytes
 * 64 words *
 */

#include <uso/sleep.h>

#include <dev/chips/ee_93C46.h>
#include <dev/spi.h>
#include <dev/err.h>

/*
 * MICROWIRE COMMANDS 
 */

/* commands without data uses the two higher bits of the address */
#define _MW_EWDS  (0x0)  /* disable write mode */
#define _MW_WRAL  (0x1)  /* write all */
#define _MW_ERAL  (0x2)  /* erase all */
#define _MW_EWEN  (0x3)  /* enabele write mode */

#define _MW_WRITE (0x4)
#define _MW_READ  (0x8)
#define _MW_ERASE (0xC)

#define _MW_COMMAND(c) ((c)<<4)

static void
chip_ready (void)
{
    /*
     * do not use polling! missing pull up! 
     */
    USO_sleep (USO_MSEC_2_TICKS (20));
}

static void
ee_93c46_write_short (DEV_spi_dev_t *spi, unsigned short addr, unsigned short data)
{
    spi->cs = DEV_SPI_DEV_CS_ACTIVE;
	DEV_spi_exchange (spi, (addr & 0x3f) | _MW_COMMAND (_MW_WRITE), NULL);
    DEV_spi_exchange (spi, data >> 8, NULL);
    spi->cs = DEV_SPI_DEV_CS_PASSIVE;
    DEV_spi_exchange (spi, data, NULL);
    chip_ready();
}

static unsigned short
ee_93c46_read_short (DEV_spi_dev_t *spi, unsigned short addr)
{
    unsigned long data = 0;
    spi->cs = DEV_SPI_DEV_CS_ACTIVE;
    DEV_spi_exchange (spi, (addr & 0x3f) | _MW_COMMAND (_MW_READ), NULL);
    DEV_spi_exchange (spi, 0, &data);
    spi->cs = DEV_SPI_DEV_CS_PASSIVE;
    DEV_spi_exchange (spi, 0, &data);
    return ((unsigned short)data);
}



extern ACE_err_t
EE_93C46_init (EE_93C46_t *ee, DEV_spi_dev_t *spi)
{
    ACE_err_t err = DEV_ERR;

    ee->spi = spi;
    if (0 == DEV_spi_dev_config (spi,
                                 0x1F,     /* clock */
                                 0,     /* mode */
                                 8,     /* word size */
                                 0,     /* delay_b_sclk */
                                 0,     /* delay_b_ct */
                                 DEV_SPI_DEV_CS_PASSIVE))
    {
        err = ACE_OK;
    }
    return err;
}

extern void
EE_93C46_eraseall (EE_93C46_t *ee)
{
    DEV_spi_acquire(ee->spi);
    DEV_spi_exchange(ee->spi, _MW_COMMAND (_MW_EWEN), NULL);
    DEV_spi_exchange(ee->spi, _MW_COMMAND (_MW_ERAL), NULL);
    chip_ready ();
    DEV_spi_exchange(ee->spi, _MW_COMMAND (_MW_EWDS), NULL);
    DEV_spi_release(ee->spi);
}

extern void
EE_93C46_write (EE_93C46_t *ee, unsigned int addr, unsigned int len, unsigned short *data)
{
    unsigned int i;
    DEV_spi_acquire(ee->spi);
    addr /= 2;
    len /= 2;
    DEV_spi_exchange(ee->spi, _MW_COMMAND (_MW_EWEN), NULL);
    for (i = len; i; i--, data++, addr++)
    {
        ee_93c46_write_short (ee->spi, addr,*data);
    }
    DEV_spi_exchange(ee->spi, _MW_COMMAND (_MW_EWDS), NULL);
    DEV_spi_release(ee->spi);
}

extern unsigned int
EE_93C46_read (EE_93C46_t *ee, unsigned int addr, unsigned int len, unsigned short *data)
{
    unsigned int i;
    DEV_spi_acquire(ee->spi);
    addr /= 2;
    len /= 2;
    for (i = len; i; i--, data++, addr++)
    {
        *data = ee_93c46_read_short (ee->spi ,addr);
    }
    DEV_spi_release(ee->spi);
    return len;
}
