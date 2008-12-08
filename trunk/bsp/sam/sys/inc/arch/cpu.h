#ifndef SAM_CPU_H
#define SAM_CPU_H

#include "dev/arch/at91/AT91SAM7X256.h"

#define ARM_MODE_ABT     0x17
#define ARM_MODE_FIQ     0x11
#define ARM_MODE_IRQ     0x12
#define ARM_MODE_SVC     0x13

#define I_BIT            0x80
#define F_BIT            0x40

#define AT91B_MAIN_OSC        18432000               // Main Oscillator MAINCK
#define AT91B_MCK             ((18432000*73/14)/2)   // Output PLL Clock (48 MHz)
#define AT91B_MASTER_CLOCK    AT91B_MCK

#endif
