#include <arch/pins.h>

extern void
LA2_pins_init (void)
{

    /* Enable the peripheral clock. */
    UPIO->ECR = 1;

    /* Configure pins as output */
    UPIO->OER    = STAT_LED | LAN_RESET | RED_LED | GREEN_LED |
                   M1_A | M1_B | M2_A | M2_B |
                   LED_BAR1_CLK | LED_BAR1_DATA | LED_BAR2_CLK | LED_BAR2_DATA |
                   OUTPUT_1 | OUTPUT_2;

    /* Set high - (negative logic outputs to be off) */
    UPIO->SODR   = STAT_LED | RED_LED | GREEN_LED;

}

extern void
LA2_pins_cfg_out(ACE_u32_t pin, int state)
{
    UPIO->OER = pin;
    if (state){
        UPIO->SODR = pin;
    } else {
        UPIO->CODR = pin;
    }
}

extern void
LA2_pins_cfg_in(ACE_u32_t pin)
{
    UPIO->ODR = pin;
}
