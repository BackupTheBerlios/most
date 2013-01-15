/*
 * io_spi.h
 *
 *  Created on: 26.07.2012
 *      Author: maik
 */

#ifndef DEV_IO_SPI_H_
#define DEV_IO_SPI_H_

#include <ace/stddef.h>
#include <dev/spi.h>
#include <dev/digout.h>
#include <dev/digin.h>
#include <uso/cpu.h>

/** @addtogroup dev
 *
 * @{
 */

/** @defgroup io_spi io_spi.h
 *
 * spi realized with io port pins
 *
 * @{
 */



/**
 *
 */
#define DEV_IO_SPI_SLAVE     0
/**
 *
 */
#define DEV_IO_SPI_MASTER    1

struct DEV_io_spi_bus
{
    DEV_spi_bus_t bus;
    DEV_digout_t *clk;
    DEV_digout_t *dout;
    DEV_digout_t *cs;
    DEV_digin_t *din;
    ACE_bool_t start;
    USO_cpu_status_t ps;
};

/**
 *
 */
typedef struct DEV_io_spi_bus DEV_io_spi_bus_t;



struct DEV_io_spi_dev
{
    DEV_spi_dev_t dev;
    DEV_io_spi_bus_t *bus;
};

/**
 *
 */
typedef struct DEV_io_spi_dev DEV_io_spi_dev_t;



/**
 *
 */
extern int DEV_io_spi_bus_init (DEV_io_spi_bus_t * io_spi_bus,
                                DEV_digout_t *spi_clk,
                                DEV_digout_t *spi_do,
                                DEV_digout_t *spi_cs,
                                DEV_digin_t *spi_di);

/**
 *
 */
extern int DEV_io_spi_dev_init (DEV_io_spi_dev_t * io_spi_dev);


#endif /* IO_SPI_H_ */
