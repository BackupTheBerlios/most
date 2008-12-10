#include "arch/pins.h"
#include <dev/arch/at91/lib_AT91SAM7X256.h>

void SAM_pins_init (void)
{
	// for RunLED
	AT91C_BASE_PIOB->PIO_OER = (AT91B_RUN_LED);		// set to output
	AT91C_BASE_PIOB->PIO_PER = (AT91B_RUN_LED);		// set to PIO mode
	AT91C_BASE_PIOB->PIO_PPUDR = (AT91B_RUN_LED);	// disable pull up

	// for GreenLED
	AT91C_BASE_PIOA->PIO_OER = (AT91A_GREEN_LED);		// set to output
	AT91C_BASE_PIOA->PIO_PER = (AT91A_GREEN_LED);		// set to PIO mode
	AT91C_BASE_PIOA->PIO_PPUDR = (AT91A_GREEN_LED);	// disable pull up

	// for RedLED
	AT91C_BASE_PIOA->PIO_OER = (AT91A_RED_LED);		// set to output
	AT91C_BASE_PIOA->PIO_PER = (AT91A_RED_LED);		// set to PIO mode
	AT91C_BASE_PIOA->PIO_PPUDR = (AT91A_RED_LED);	// disable pull up

	// for LCD Backlight
	AT91C_BASE_PIOB->PIO_OER = (AT91B_LCD_BL);		// set to output
	AT91C_BASE_PIOB->PIO_PER = (AT91B_LCD_BL);		// set to PIO mode
	AT91C_BASE_PIOB->PIO_PPUDR = (AT91B_LCD_BL);	// disable pull up

	// for LCD Reset
	AT91C_BASE_PIOA->PIO_OER = (AT91A_LCD_RST);		// set to output
	AT91C_BASE_PIOA->PIO_PER = (AT91A_LCD_RST);		// set to PIO mode

	// for PHY power down
	AT91C_BASE_PIOB->PIO_OER = (AT91B_PHY_POWER_DOWN);		// set to output
	AT91C_BASE_PIOB->PIO_PER = (AT91B_PHY_POWER_DOWN);		// set to PIO mode
	AT91C_BASE_PIOB->PIO_PPUDR = (AT91B_PHY_POWER_DOWN);	// disable pull up

	/* disable pull up on RXDV => PHY normal mode (not in test mode),
	PHY has internal pull down. */
	AT91C_BASE_PIOB->PIO_PPUDR = 1 << 15;

	/* PHY has internal pull down : set MII mode. */
	AT91C_BASE_PIOB->PIO_PPUDR= 1 << 16;
	
	AT91C_BASE_PIOA->PIO_ODR = (AT91A_JS_ALL);			// set PIO line to input

    	/* Enable the peripheral clock. */
	AT91F_PMC_EnablePeriphClock( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );
	AT91F_PMC_EnablePeriphClock( AT91C_BASE_PMC, 1 << AT91C_ID_PIOB ) ;
	AT91F_PMC_EnablePeriphClock( AT91C_BASE_PMC, 1 << AT91C_ID_EMAC ) ;

}
