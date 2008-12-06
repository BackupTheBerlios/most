#ifndef LIB_AT91SAM7X256_H_
#define LIB_AT91SAM7X256_H_

#include <ace/stddef.h>
#include <dev/arch/at91/AT91SAM7X256.h>

void AT91F_PIO_CfgPeriph(
	AT91PS_PIO pPio,             // \arg pointer to a PIO controller
	unsigned int periphAEnable,  // \arg PERIPH A to enable
	unsigned int periphBEnable)  // \arg PERIPH B to enable

{
	pPio->PIO_ASR = periphAEnable;
	pPio->PIO_BSR = periphBEnable;
	pPio->PIO_PDR = (periphAEnable | periphBEnable); // Set in Periph mode
}

void AT91F_EMAC_CfgPIO (void)
{
	// Configure PIO controllers to periph mode
	AT91F_PIO_CfgPeriph(
		AT91C_BASE_PIOB, // PIO controller base address
		((unsigned int) AT91C_PB2_ETX0    ) |
		((unsigned int) AT91C_PB12_ETXER   ) |
		((unsigned int) AT91C_PB16_ECOL    ) |
		((unsigned int) AT91C_PB11_ETX3    ) |
		((unsigned int) AT91C_PB6_ERX1    ) |
		//(unsigned int) AT91C_PB15_ERXDV   ) |
		((unsigned int) AT91C_PB15_ERXDV_ECRSDV ) |
		((unsigned int) AT91C_PB13_ERX2    ) |
		((unsigned int) AT91C_PB3_ETX1    ) |
		((unsigned int) AT91C_PB8_EMDC    ) |
		((unsigned int) AT91C_PB5_ERX0    ) |
		//((unsigned int) AT91C_PB18_EF100   ) |
		((unsigned int) AT91C_PB14_ERX3    ) |
		//((unsigned int) AT91C_PB4_ECRS_ECRSDV) |
		((unsigned int) AT91C_PB4_ECRS) |
		((unsigned int) AT91C_PB1_ETXEN   ) |
		((unsigned int) AT91C_PB10_ETX2    ) |
		((unsigned int) AT91C_PB0_ETXCK_EREFCK) |
		((unsigned int) AT91C_PB9_EMDIO   ) |
		((unsigned int) AT91C_PB7_ERXER   ) |
		((unsigned int) AT91C_PB17_ERXCK   ), // Peripheral A
		0); // Peripheral B
}

void AT91F_PMC_EnablePeriphClock (
	AT91PS_PMC pPMC, // \arg pointer to PMC controller
	unsigned int periphIds)  // \arg IDs of peripherals to enable
{
	pPMC->PMC_PCER = periphIds;
}

#endif /*LIB_AT91SAM7X256_H_*/
