#ifndef DEV_AT91_SPI_H
#define DEV_AT91_SPI_H

#include <ace/stddef.h>
#include "dev/spi.h"
#include "dev/arch/at91sam7x/AT91SAM7X256.h"

#define AT91C_SPI_SLAVE		0
#define	AT91C_SPI_MASTER	1

struct DEV_at91_spi_bus
{
    DEV_spi_bus_t bus;
    AT91PS_SPI base;
    int id;
    int ctrl;
    int devices;
};

typedef struct DEV_at91_spi_bus DEV_at91_spi_bus_t;



struct DEV_at91_spi_dev
{
    DEV_spi_dev_t dev;
    DEV_at91_spi_bus_t *bus;
};

typedef struct DEV_at91_spi_dev DEV_at91_spi_dev_t;



extern int DEV_at91_SPI_bus_init (DEV_at91_spi_bus_t * at91, int devices);

extern int DEV_at91_SPI_dev_init (DEV_at91_spi_dev_t * at91);

#endif /*SDEV_AT91_SPI_H */
