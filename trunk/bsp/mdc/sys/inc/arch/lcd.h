/*
 * lcd.h
 *
 *  Created on: 31.08.2013
 *      Author: maik
 */

#ifndef MDC_LCD_H_
#define MDC_LCD_H_

#include <dpy/ks0070b.h>
#include <ace/stddef.h>

extern DPY_ks0070b_t lcd;

extern void MDC_lcd_init (void);

extern void MDC_lcd_backlight_on(ACE_bool_t on);

#endif /* MDC_LCD_H_ */
