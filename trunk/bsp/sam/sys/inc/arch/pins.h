#ifndef SAM_PINS_H
#define SAM_PINS_H

#include <dev/arch/at91/AT91SAM7X256.h>

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

#define AT91B_RUN_LED       AT91C_PIO_PB21      /**< Run LED */
#define AT91A_GREEN_LED     AT91C_PIO_PA3       /**< Green LED */
#define AT91A_RED_LED       AT91C_PIO_PA4       /**< Red LED */

#define AT91B_PHY_POWER_DOWN	AT91C_PIO_PB18  /**< PHY power down */

#define AT91B_LCD_BL		AT91C_PIO_PB20  /**< LCD Backlight */
#define AT91A_LCD_RST		AT91C_PIO_PA2   /**< LCD Reset */

#define AT91B_WP            AT91C_PIO_PB22      /**< Write protect */
#define AT91B_CP            AT91C_PIO_PB23      /**< Card protect */
#define AT91B_SW1           AT91C_PIO_PB24      /**< Switch 1 */
#define AT91B_SW2           AT91C_PIO_PB25      /**< Switch 2 */
#define AT91B_SW_BOOT       AT91C_PIO_PB28      /**< Switch boot */

/**
 *
 */
#define AT91B_INPUT_ALL       (AT91B_WP | AT91B_CP | AT91B_SW1 | AT91B_SW2 | AT91B_SW_BOOT)

#define AT91A_JS_LEFT	    AT91C_PIO_PA7       /**< Joystick     left */
#define AT91A_JS_DOWN	    AT91C_PIO_PA8       /**<                      down */
#define AT91A_JS_UP		    AT91C_PIO_PA9       /**<                      up */
#define AT91A_JS_RIGHT	    AT91C_PIO_PA14      /**<                      right */
#define AT91A_JS_CENTER	    AT91C_PIO_PA15      /**<                      center */

/**
 *
 */
#define AT91A_INPUT_ALL       (AT91A_JS_LEFT | AT91A_JS_RIGHT | AT91A_JS_UP | AT91A_JS_DOWN | AT91A_JS_CENTER)

/**
 *
 */
extern void SAM_pins_init (void);

/** @}
 */

/** @}
 */

#endif
