#include <dev/arch/at91sam7a2/csp.h>

#include <arch/pins.h>

void
LA2_pins_init (void)
{

    /* Enable the peripheral clock. */
    UPIO->ECR = 1;

    // Led as output
    UPIO->OER    = STAT_LED | LAN_RESET | RED_LED | GREEN_LED |
                   M1_A | M1_B | M2_A | M2_B |
                   LED_BAR1_CLK | LED_BAR1_DATA | LED_BAR2_CLK | LED_BAR2_DATA;

    // Set high - turn off
    UPIO->SODR   = STAT_LED | RED_LED | GREEN_LED;

}