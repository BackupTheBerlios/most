#ifndef LIB_AT91SAM7X256_H_
#define LIB_AT91SAM7X256_H_

#include <ace/stddef.h>

#include "dev/arch/at91/AT91SAM7X256.h"


void AT91F_PIO_CfgPeriph(
	AT91PS_PIO pPio,             // \arg pointer to a PIO controller
	unsigned int periphAEnable,  // \arg PERIPH A to enable
	unsigned int periphBEnable);  // \arg PERIPH B to enable

void AT91F_EMAC_CfgPIO (void);

void AT91F_PMC_EnablePeriphClock (
	AT91PS_PMC pPMC, // \arg pointer to PMC controller
	unsigned int periphIds);  // \arg IDs of peripherals to enable

#endif /*LIB_AT91SAM7X256_H_*/
