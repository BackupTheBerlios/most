/*
 * interrupts.h
 *
 *  Created on: 26.11.2013
 *      Author: maik
 */

#ifndef MDC_INTERRUPTS_H_
#define MDC_INTERRUPTS_H_

#include <dev/chips/smc_91c94.h>

extern void MDC_interrupts_init(void);

extern void MDC_interrupts_set_eth(void (*f)(DEV_smc91c94_t *), DEV_smc91c94_t *smc);


#endif /* INTERRUPTS_H_ */
