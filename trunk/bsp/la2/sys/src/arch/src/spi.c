/*
 * spi.c
 *
 *  Created on: 26.04.2010
 *      Author: maik
 */

#include <arch/spi.h>

static SAM7A2_spi_bus_t spi_0;

SAM7A2_spi_dev_t LA2_mmc;

extern int
LA2_spi_init (void)
{
    if (0 != DEV_spi_bus_init ((DEV_spi_bus_t *) & spi_0, 0, DEV_SPI_BUS_CTRL_MASTER, 1))
        return -1;
    if (0 != DEV_spi_dev_init ((DEV_spi_dev_t *) & LA2_mmc, (DEV_spi_bus_t *) & spi_0, 1))
        return -1;

    if (0 != SAM7A2_spi_bus_init (&spi_0, (NPCS1)))
        return -1;
    if (0 != SAM7A2_spi_dev_init (&LA2_mmc))
        return -1;

    return 0;
}
