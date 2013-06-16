#ifndef LA2_PINS_H
#define LA2_PINS_H

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


#define MMC_WP   	PIO0
#define MMC_CP   	PIO1
#define STAT_LED   	PIO2
#define RUN_LED   	STAT_LED
#define RED_LED   	STAT_LED
#define GREEN_LED   STAT_LED
#define LAN_RESET   PIO3

/**
 *
 */
extern void LA2_pins_init (void);

/** @}
 */

/** @}
 */

#endif
