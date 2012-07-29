/*
 * spi.h
 *
 *  Created on: 26.04.2010
 *      Author: maik
 */

#ifndef SPI_H_
#define SPI_H_

#include <dev/arch/at91/spi.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup spi spi.h
 *
 * Serial peripheral interface.
 *
 * @{
 */

/**
 *
 */
extern DEV_at91_spi_dev_t SAM_display;

/**
 *
 */
extern DEV_at91_spi_dev_t SAM_mmc;

/**
 *
 */
extern int SAM_spi_init (void);

/** @}
 */

/** @}
 */

#endif /* SPI_H_ */
