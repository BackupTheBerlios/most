/*
 * io_spi.c
 *
 *  Created on: 26.07.2012
 *      Author: maik
 */

#include <dev/io_spi.h>
#include <dev/digin.h>


static int io_spi_dev_config (DEV_spi_dev_t * dev);

static int io_spi_exchange (DEV_spi_dev_t * dev, ACE_u32_t out, ACE_u32_t * in);

static void
chip_select(DEV_io_spi_bus_t *bus)
{
    DEV_digout_clear(bus->clk);
    DEV_digout_set(bus->cs);
    bus->startbit = TRUE;
}

#if 0
static void
chip_release(DEV_io_spi_bus_t *bus)
{
    DEV_digout_clear(bus->clk);
    DEV_digout_clear(bus->cs);
    DEV_digout_clear(bus->dout);
}
#endif

static void
startbit(DEV_io_spi_bus_t *bus)
{
    DEV_digout_set(bus->dout);
    DEV_digout_set(bus->clk);
    DEV_digout_clear(bus->clk);
    bus->startbit = FALSE;
}

static int
io_spi_dev_config (DEV_spi_dev_t * dev)
{
    DEV_io_spi_dev_t *io_spi_dev = (DEV_io_spi_dev_t *) dev;

    // TODO slave device handling

    if (dev->bus->ctrl != DEV_SPI_BUS_CTRL_MASTER)
        return (-1);

    if (dev->cs_active == DEV_SPI_DEV_CS_ACTIVE)
    {
        chip_select(io_spi_dev->bus);
    }


    return 0;
}

static int
io_spi_exchange (DEV_spi_dev_t * dev, ACE_u32_t out, ACE_u32_t * in)
{
    ACE_u32_t i = 1;
    DEV_io_spi_dev_t *io_spi_dev = (DEV_io_spi_dev_t *) dev;
    DEV_io_spi_bus_t *bus = io_spi_dev->bus;

    if (bus->startbit == TRUE)
    {
        startbit(bus);
    }

    out &= dev->word_mask;

    for (i <<= (dev->word_size-1); i; i >>= 1)
    {
        *in <<= 1;
        if (out & i)
        {
            DEV_digout_set(bus->dout);
        }
        else
        {
            DEV_digout_clear(bus->dout);
        }
        DEV_digout_set(bus->clk);
        *in |= (DEV_digin_isset(bus->din) == TRUE ? 1 : 0);
        DEV_digout_clear(bus->clk);
    }

    return 0;
}


extern int
DEV_io_spi_bus_init (DEV_io_spi_bus_t * io_spi_bus,
                    DEV_digout_t *spi_clk,
                    DEV_digout_t *spi_do,
                    DEV_digout_t *spi_cs,
                    DEV_digin_t *spi_di)
{
    DEV_spi_bus_t *bus = (DEV_spi_bus_t *) io_spi_bus;
    bus->exchange = io_spi_exchange;
    bus->dev_cfg = io_spi_dev_config;
    io_spi_bus->clk = spi_clk;
    io_spi_bus->dout = spi_do;
    io_spi_bus->cs = spi_cs;
    io_spi_bus->din = spi_di;
    io_spi_bus->startbit = FALSE;
    return 0;
}

extern int
DEV_io_spi_dev_init (DEV_io_spi_dev_t * io_spi_dev)
{
    DEV_spi_dev_t *dev = (DEV_spi_dev_t *) io_spi_dev;
    io_spi_dev->bus = (DEV_io_spi_bus_t *) (dev->bus);
    return 0;
}

