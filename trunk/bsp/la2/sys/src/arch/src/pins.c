#include <dev/arch/at91sam7a2/csp.h>

#include <arch/pins.h>

void
LA2_pins_init (void)
{

    /* Enable the peripheral clock. */
    UPIO->ECR = 1;

    // Led as output
    UPIO->OER    = STAT_LED | LAN_RESET;

    // Set high - turn off
    UPIO->SODR   = STAT_LED;

}
