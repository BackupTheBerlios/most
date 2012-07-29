/*
 * spi.h
 *
 *  Created on: 27.07.2012
 *      Author: maik
 */

#ifndef SPI_H_
#define SPI_H_

#include <dev/io_spi.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup spi spi.h
 *
 * Serial Periphral Interfaces
 *
 * @{
 */


/**
 *
 */
extern DEV_io_spi_dev_t MDC_spi_ee;

/**
 *
 */
extern int MDC_spi_init (void);

/** @}
 */

/** @}
 */

#endif /* SPI_H_ */
