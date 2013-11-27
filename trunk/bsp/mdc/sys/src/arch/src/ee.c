/*
 * ee.c
 *
 *  Created on: 27.11.2013
 *      Author: maik
 */

#include <ace/err.h>

#include <arch/spi.h>
#include <arch/ee.h>


EE_93C46_t ee;

extern ACE_err_t
MDC_ee_init(void)
{
    return EE_93C46_init(&ee, (DEV_spi_dev_t *)(&MDC_spi_ee));
}


