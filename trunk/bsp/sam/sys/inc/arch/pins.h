#ifndef SAM_PINS_H
#define SAM_PINS_H

#include <dev/arch/at91sam7x/AT91SAM7X256.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup pins pins.h
 *
 * Pins.
 *
 * @{
 */


#define A_LCD_RST		AT91C_PIO_PA2       /**< LCD Reset */
#define A_GREEN_LED     AT91C_PIO_PA3       /**< Green LED */
#define A_RED_LED       AT91C_PIO_PA4       /**< Red LED */
#define A_JS_LEFT	    AT91C_PIO_PA7       /**< Joystick     left */
#define A_JS_DOWN	    AT91C_PIO_PA8       /**<                      down */
#define A_JS_UP		    AT91C_PIO_PA9       /**<                      up */
#define A_JS_RIGHT	    AT91C_PIO_PA14      /**<                      right */
#define A_JS_CENTER	    AT91C_PIO_PA15      /**<                      center */

/**
 *
 */
#define A_INPUT_ALL       (A_JS_LEFT | A_JS_RIGHT | A_JS_UP | A_JS_DOWN | A_JS_CENTER)


/**
 *
 */
#define A_OUTPUT_PULL_UP       (A_LCD_RST)
#define A_OUTPUT_DIS_PULL_UP   (A_GREEN_LED | A_RED_LED)

/**
 *
 */
#define A_OUTPUT_ALL       (A_OUTPUT_PULL_UP | A_OUTPUT_DIS_PULL_UP)



#define B_PHY_POWER_DOWN	AT91C_PIO_PB18      /**< PHY power down */
#define B_LCD_BL		    AT91C_PIO_PB20      /**< LCD Backlight */
#define B_RUN_LED           AT91C_PIO_PB21      /**< Run LED */
#define B_WP                AT91C_PIO_PB22      /**< Write protect */
#define B_CP                AT91C_PIO_PB23      /**< Card protect */
#define B_SW1               AT91C_PIO_PB24      /**< Switch 1 */
#define B_SW2               AT91C_PIO_PB25      /**< Switch 2 */
#define B_PIN_B27          	AT91C_PIO_PB27      /**< IO B27 / AD0   (Input)*/
#define B_SW_BOOT           AT91C_PIO_PB28      /**< Switch boot */
#define B_PIN_B29          	AT91C_PIO_PB29      /**< IO B29 / AD2 / PWM2 (Output)*/
#define B_PIN_B30       	AT91C_PIO_PB30      /**< IO B30 / AD3 / PWM3 (PWM) */

/**
 *
 */
#define B_INPUT_ALL       (B_WP | B_CP | B_SW1 | B_SW2 | B_PIN_B27 | B_SW_BOOT)

/**
 *
 */
#define B_OUTPUT_PULL_UP       (B_PIN_B29)
#define B_OUTPUT_DIS_PULL_UP   (B_PHY_POWER_DOWN | B_LCD_BL | B_RUN_LED)

/**
 *
 */
#define B_OUTPUT_ALL       (B_OUTPUT_PULL_UP | B_OUTPUT_DIS_PULL_UP)




/**
 *
 */
extern void SAM_pins_init (void);

/** @}
 */

/** @}
 */

#endif
