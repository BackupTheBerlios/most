/*
 * spi.h
 *
 *  Created on: 26.04.2010
 *      Author: maik
 */

#ifndef SPI_H_
#define SPI_H_

#include <dev/arch/at91sam7a2/csp.h>
#include <dev/arch/at91sam7a2/types.h>
#include <dev/arch/at91sam7a2/spi.h>

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
extern SAM7A2_spi_dev_t LA2_mmc;

/**
 *
 */
extern int LA2_spi_init (void);

/** @}
 */

/** @}
 */

#endif /* SPI_H_ */
