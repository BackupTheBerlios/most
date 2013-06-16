#include <dev/arch/at91sam7x/pmc.h>

#include "arch/pins.h"

void
SAM_pins_init (void)
{
    // port A
    AT91C_BASE_PIOA->PIO_ODR = (A_INPUT_ALL);       // set PIO line to input
    AT91C_BASE_PIOA->PIO_OER = (A_OUTPUT_ALL);      // set to output
    AT91C_BASE_PIOA->PIO_PPUDR = (A_INPUT_ALL | A_OUTPUT_DIS_PULL_UP); // disable pull up
    AT91C_BASE_PIOA->PIO_PER = (A_INPUT_ALL | A_OUTPUT_ALL);      // set to PIO mode

    // port B
    AT91C_BASE_PIOB->PIO_ODR = (B_INPUT_ALL);       // set PIO line to input
    AT91C_BASE_PIOB->PIO_OER = (B_OUTPUT_ALL);      // set to output
    AT91C_BASE_PIOB->PIO_PPUDR = (B_INPUT_ALL | B_OUTPUT_DIS_PULL_UP); // disable pull up
    AT91C_BASE_PIOB->PIO_PER = (B_INPUT_ALL | B_OUTPUT_ALL );   // set to PIO mode



    // PHY
    /* disable pull up on RXDV => PHY normal mode (not in test mode), PHY has internal pull down. */
    AT91C_BASE_PIOB->PIO_PPUDR = 1 << 15;
    /* PHY has internal pull down : set MII mode. */
    AT91C_BASE_PIOB->PIO_PPUDR = 1 << 16;

    // PWM speaker
    AT91C_BASE_PIOB->PIO_PDR = 1 << 19;
    AT91C_BASE_PIOB->PIO_ASR = 1 << 19;
    AT91C_BASE_PIOB->PIO_BSR = 0;

    /* Enable the peripheral clock. */
    DEV_at91_PMC_enable_peripheral (AT91C_ID_PIOA);
    DEV_at91_PMC_enable_peripheral (AT91C_ID_PIOB);
    DEV_at91_PMC_enable_peripheral (AT91C_ID_EMAC);
    DEV_at91_PMC_enable_peripheral (AT91C_ID_PWMC);

}
