#ifndef DPY_NOKIA6100_H
#define DPY_NOKIA6100_H

#include <ace/stddef.h>
#include "dev/digout.h"
#include "dev/spi.h"

/** @addtogroup dpy
 *
 * @{
 */

/** @addtogroup nokia6100
 *
 * @{
 */

/** @defgroup nokia6100_i nokia6100.h
 *
 * Window commands.
 *
 * @{
 */


// Booleans
#define DPY_NOKIA6100_NOFILL              0 /**< */
#define DPY_NOKIA6100_FILL                1 /**< */

// 12-bit color definitions
#define DPY_NOKIA6100_WHITE          0xFFF /**< */
#define DPY_NOKIA6100_BLACK          0x000 /**< */
#define DPY_NOKIA6100_RED            0xF00 /**< */
#define DPY_NOKIA6100_GREEN          0x0F0 /**< */
#define DPY_NOKIA6100_BLUE           0x00F /**< */
#define DPY_NOKIA6100_CYAN           0x0FF /**< */
#define DPY_NOKIA6100_MAGENTA        0xF0F /**< */
#define DPY_NOKIA6100_YELLOW         0xFF0 /**< */
#define DPY_NOKIA6100_BROWN          0xB22 /**< */
#define DPY_NOKIA6100_ORANGE         0xFA0 /**< */
#define DPY_NOKIA6100_PINK           0xF6A /**< */

// Font sizes
#define DPY_NOKIA6100_SMALL          0 /**< */
#define DPY_NOKIA6100_MEDIUM         1 /**< */
#define DPY_NOKIA6100_LARGE          2 /**< */

/**
 *
 */
extern int DPY_nokia6100_spi_init (DEV_spi_dev_t * dev);

/**
 *
 */
extern void DPY_nokia6100_init (DEV_spi_dev_t * dev);

/**
 *
 */
extern void DPY_nokia6100_reset (DEV_digout_t * reset);

/**
 *
 */
extern void DPY_nokia6100_backlight (DEV_digout_t * backlight, ACE_bool_t state);

/**
 *
 */
extern void DPY_nokia6100_clear_screen (void);

/**
 *
 */
extern void DPY_nokia6100_set_contrast (unsigned int contrast);

/**
 *
 */
extern void DPY_nokia6100_set_pixel (int x, int y, int color);

/**
 *
 */
extern void DPY_nokia6100_set_line (int x1, int y1, int x2, int y2, int color);

/**
 *
 */
extern void DPY_nokia6100_set_rectangle (int x1, int y1, int x2, int y2, unsigned char fill,
                                         int color);

/**
 *
 */
extern void DPY_nokia6100_set_circle (int x, int y, int radius, int color);

/**
 *
 */
extern void DPY_nokia6100_put_char (char c, int x, int y, int size, int fcolor, int bcolor);

/**
 *
 */
extern void DPY_nokia6100_put_string (char *string, int x, int y, int size, int fcolor, int bcolor);

/**
 *
 */
extern void DPY_nokia6100_write_bitmap (const unsigned char bmp[], int x1, int x2, int y1, int y2,
                                        int size);

/** @}
 */

/** @}
 */

/** @}
 */


#endif /*DPY_NOKIA6100_H */
