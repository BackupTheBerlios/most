/*
 * spi.c
 *
 *  Created on: 27.07.2012
 *      Author: maik
 */


#include "arch/spi.h"
#include "arch/pins.h"
#include "dev/digio.h"
#include <dev/arch/h8/bitops.h>
#include <dev/arch/h8/H83003_REG.h>

#define CS_PORT   H8_P6DR
#define CS_PIN    H8_P60_EE_CS

#define CLK_PORT  H8_P6DR
#define CLK_PIN   H8_P62_EE_CLK

#define DI_PORT   H8_P7DR
#define DI_PIN    H8_P77_EE_DI

#define DO_PORT   H8_P6DR
#define DO_PIN    H8_P61_EE_DO

static DEV_digout_t spi_clk;
static DEV_digout_t spi_do;
static DEV_digout_t spi_cs;
static DEV_digin_t spi_di;

static DEV_io_spi_bus_t spi;
DEV_io_spi_dev_t MDC_spi_ee;

static void
set_spi_clk (void)
{
    H8_BITSET(CLK_PIN,CLK_PORT);
}

static void
clear_spi_clk (void)
{
    H8_BITCLEAR(CLK_PIN,CLK_PORT);
}

static void
set_spi_do (void)
{
    H8_BITSET (DO_PIN, DO_PORT);
}

static void
clear_spi_do (void)
{
    H8_BITCLEAR(DO_PIN,DO_PORT);
}

static void
set_spi_cs (void)
{
    H8_BITSET(CS_PIN,CS_PORT);
}

static void
clear_spi_cs (void)
{
    H8_BITCLEAR(CS_PIN,CS_PORT);
}

static unsigned long
sample_spi_di (void)
{
    return DI_PORT & DI_PIN;
}

extern int
MDC_spi_init (void)
{
    DEV_digout_init (&spi_clk, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_spi_clk, clear_spi_clk);
    DEV_digout_init (&spi_do, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_spi_do, clear_spi_do);
    DEV_digout_init (&spi_cs, DEV_DIGIO_LOW, DEV_DIGIO_POS, set_spi_cs, clear_spi_cs);
    DEV_digin_init (NULL, &spi_di, DEV_DIGIO_POS, sample_spi_di, 0);

    if (0 != DEV_spi_bus_init ((DEV_spi_bus_t *) &spi, 0, DEV_SPI_BUS_CTRL_MASTER, 1))
        return -1;
    if (0 != DEV_spi_dev_init ((DEV_spi_dev_t *) &MDC_spi_ee, (DEV_spi_bus_t *) &spi, 0))
        return -1;

    if (0 != DEV_io_spi_bus_init (&spi, &spi_clk, &spi_do, &spi_cs, &spi_di))
        return -1;
    if (0 != DEV_io_spi_dev_init (&MDC_spi_ee))
        return -1;

    return 0;
}
