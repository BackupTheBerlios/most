/*
 * spi.c
 *
 *  Created on: 26.04.2010
 *      Author: maik
 */

#include "arch/spi.h"

static DEV_at91_spi_bus_t spi_0;
//static DEV_at91_spi_bus_t spi_1;

DEV_at91_spi_dev_t SAM_display;
DEV_at91_spi_dev_t SAM_mmc;

extern int SAM_spi_init(void)
{
	if (0 != DEV_spi_bus_init((DEV_spi_bus_t *)&spi_0, 0, DEV_SPI_BUS_CTRL_MASTER, 1))
		return -1;
	if (0 != DEV_spi_dev_init((DEV_spi_dev_t *)&SAM_display, (DEV_spi_bus_t *)&spi_0, 0))
		return -1;
	if (0 != DEV_spi_dev_init((DEV_spi_dev_t *)&SAM_mmc, (DEV_spi_bus_t *)&spi_0, 1))
		return -1;

	if (0 != DEV_at91_spi_bus_init(&spi_0, (AT91C_PA12_SPI0_NPCS0 | AT91C_PA13_SPI0_NPCS1) ))
		return -1;
	if (0 !=  DEV_at91_spi_dev_init(&SAM_display))
		return -1;
	if (0 !=  DEV_at91_spi_dev_init(&SAM_mmc))
		return -1;

	return 0;
}
