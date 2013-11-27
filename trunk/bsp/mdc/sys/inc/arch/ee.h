/*
 * ee.h
 *
 *  Created on: 27.11.2013
 *      Author: maik
 */

#ifndef MDC_EE_H_
#define MDC_EE_H_

#include <ace/err.h>
#include <dev/chips/ee_93C46.h>


/*
 * EE works only with words !
 */



/**
 * serial ee prom 1Kbit
 * 1024 / 8 = 128 Bytes
 */
extern EE_93C46_t ee;

/**
 * Initialize eeprom communication.
 */
extern ACE_err_t MDC_ee_init(void);



#endif /* EE_H_ */
