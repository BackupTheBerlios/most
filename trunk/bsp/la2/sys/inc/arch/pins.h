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


#define MMC_WP      PIO0
#define MMC_CP      PIO1
#define STAT_LED    PIO2
#define RUN_LED     STAT_LED
#define LAN_RESET   PIO3
#define RED_LED     PIO4
#define GREEN_LED   PIO5
#define SWITCH      PIO6
#define PUSHBUTTON  PIO7
#define M1_A        PIO8
#define M1_B        PIO9
#define M2_A        PIO10
#define M2_B        PIO11

#define DIO_0       PIO14
#define DIO_1       PIO15
#define DIO_2       PIO16
#define DIO_3       PIO17
#define DIO_4       PIO18
#define DIO_5       PIO19
#define DIO_6       PIO20
#define DIO_7       PIO21
#define DIO_8       PIO22
#define DIO_9       PIO23
#define DIO_10      PIO24
#define DIO_11      PIO25
#define DIO_12      PIO26
#define DIO_13      PIO27
#define DIO_14      PIO28
#define DIO_15      PIO29

#define LINE_1            DIO_0
#define LINE_2            DIO_1
#define LED_BAR1_CLK      DIO_2
#define LED_BAR1_DATA     DIO_3
#define LED_BAR2_CLK      DIO_4
#define LED_BAR2_DATA     DIO_5
#define INPUT_1           DIO_12
#define INPUT_2           DIO_13
#define OUTPUT_1          DIO_14
#define OUTPUT_2          DIO_15

/**
 *
 */
extern void LA2_pins_init (void);

/** @}
 */

/** @}
 */

#endif
