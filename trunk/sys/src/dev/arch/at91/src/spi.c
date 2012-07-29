#include "dev/arch/at91/spi.h"

static int DEV_at91_SPI_dev_config (DEV_spi_dev_t * dev);

static int DEV_at91_SPI_exchange (DEV_spi_dev_t * dev, ACE_u32_t out, ACE_u32_t * in);

extern int
DEV_at91_SPI_bus_init (DEV_at91_spi_bus_t * at91, int devices)
{
    DEV_spi_bus_t *bus = (DEV_spi_bus_t *) at91;
    volatile AT91PS_PIO pio = AT91C_BASE_PIOA;
    int p_regs;

    bus->exchange = NULL;
    bus->dev_cfg = NULL;
    at91->devices = devices;

    if (0 == bus->number)
    {
        at91->id = AT91C_ID_SPI0;
        at91->base = AT91C_BASE_SPI0;
        p_regs =
            (at91->devices | AT91C_PA16_SPI0_MISO | AT91C_PA17_SPI0_MOSI | AT91C_PA18_SPI0_SPCK);
    }
    else if (1 == bus->number)
    {
        at91->id = AT91C_ID_SPI1;
        at91->base = AT91C_BASE_SPI1;
        p_regs =
            (at91->devices | AT91C_PA24_SPI1_MISO | AT91C_PA23_SPI1_MOSI | AT91C_PA22_SPI1_SPCK);
    }
    else
        return (-1);

    if (DEV_SPI_BUS_CTRL_MASTER == bus->ctrl)
    {
        at91->ctrl = AT91C_SPI_MASTER;

        volatile AT91PS_PMC pmc = AT91C_BASE_PMC;       // pointer to PMC data structure


        // configure the slave chip selects
        pio->PIO_OER = at91->devices;   // configure slave select pins as output
        pio->PIO_SODR = at91->devices;  // set slave select pins to HIGH

        pio->PIO_PDR = p_regs;  // peripheral A disable register
        pio->PIO_ASR = p_regs;  // peripheral A select register

        // enable the spi peripheral clock
        pmc->PMC_PCER = (1 << at91->id);
    }
    else if (DEV_SPI_BUS_CTRL_SLAVE == bus->ctrl)
    {
        // TODO slave configuration
        at91->ctrl = AT91C_SPI_SLAVE;
        return (-1);
    }
    else
        return (-1);

    // spi control register SPI_CR
    at91->base->SPI_CR = AT91C_SPI_SWRST | AT91C_SPI_SPIEN;     // software reset, spi enable  (0x81)
    at91->base->SPI_CR = AT91C_SPI_SPIEN;       // spi enable (0x01)

    // spi mode register SPI_MR = 0xE0011
    at91->base->SPI_MR = (AT91C_SPI_DLYBCS & (0 << 24)) |       // delay between chip selects (take default: 6 MCK periods)
        (AT91C_SPI_PCS & (0xE << 16)) | // peripheral chip select (selects SPI_NPCS0 or PA12)
        (AT91C_SPI_LLB & (0 << 7)) |    // local loopback enabled (disabled)
        (AT91C_SPI_MODFDIS & (1 << 4)) |        // mode fault detection (disabled)
        (AT91C_SPI_PCSDEC & (0 << 2)) | // chip select decode (chip selects connected directly to peripheral)
        (AT91C_SPI_PS & (1 << 1)) |     // peripheral select
        (AT91C_SPI_MSTR & at91->ctrl);  // master or slave mode

    bus->exchange = DEV_at91_SPI_exchange;
    bus->dev_cfg = DEV_at91_SPI_dev_config;

    return 0;
}

extern int
DEV_at91_SPI_dev_init (DEV_at91_spi_dev_t * at91)
{
    DEV_spi_dev_t *dev = (DEV_spi_dev_t *) at91;
    at91->bus = (DEV_at91_spi_bus_t *) (dev->bus);
    return 0;
}

static int
DEV_at91_SPI_dev_config (DEV_spi_dev_t * dev)
{
    DEV_at91_spi_dev_t *at91 = (DEV_at91_spi_dev_t *) dev;

    // TODO slave device handling
    if (dev->bus->ctrl != DEV_SPI_BUS_CTRL_MASTER)
        return (-1);

    int ncpha = 0;
    int cpol = 0;
    switch (dev->mode)
    {
    case 0:
        ncpha = 1;
        break;
    case 1:
        break;
    case 2:
        ncpha = 1;
        cpol = 1;
        break;
    case 3:
        cpol = 1;
        break;
    default:
        break;
    }

    // spi chip select register SPI_CSR[device number(0, 1, 2, 3)]
    at91->bus->base->SPI_CSR[dev->number] = (AT91C_SPI_DLYBCT & (dev->delay_b_ct << 24)) |      // delay between consecutive transfers
        (AT91C_SPI_DLYBS & (dev->delay_b_sclk << 16)) | // delay before SPCK
        (AT91C_SPI_SCBR & (dev->sclk << 8)) |   // serial clock baud rate
        (AT91C_SPI_BITS & ((dev->word_size - 8) << 4)) |        // bits per transfer
        (AT91C_SPI_CSAAT & ((dev->cs_active == DEV_SPI_DEV_CS_ACTIVE ? 1 : 0) << 3)) |  // chip select active after transfer
        (AT91C_SPI_NCPHA & (ncpha << 1)) |      // clock phase
        (AT91C_SPI_CPOL & cpol);        // clock polarity
    return 0;
}



static int
DEV_at91_SPI_exchange (DEV_spi_dev_t * dev, ACE_u32_t out, ACE_u32_t * in)
{
    DEV_at91_spi_dev_t *at91 = (DEV_at91_spi_dev_t *) dev;

    // wait for the transfer to complete
    while ((at91->bus->base->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

    // send out
    at91->bus->base->SPI_TDR = ((AT91C_SPI_PCS & (~(0x01 << dev->number) << 16)) | (out & 0xFFFF));

    if (NULL == in)
        return 0;

    // wait for the transfer to complete
    while ((at91->bus->base->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

    // read in
    *in = (at91->bus->base->SPI_RDR & 0xFFFF);
    return 0;
}
