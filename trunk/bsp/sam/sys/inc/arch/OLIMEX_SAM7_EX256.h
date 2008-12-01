#ifndef OLIMEX_SAM7_EX256_H
#define OLIMEX_SAM7_EX256_H

#include "dev/arch/at91/AT91SAM7X256.h"

#ifndef __ASSEMBLY__
extern void dbgu_print_ascii(const char *buffer);
extern void dbgu_print_hex8(unsigned long);
#endif

#define AT91A_RUN_LED       AT91C_PIO_PA3   // Run LED
#define AT91A_RED_LED       AT91C_PIO_PA4   // Red LED
#define AT91B_LCD_BL		AT91C_PIO_PB20	// LCD Backlight
#define AT91B_LCD_RST		AT91C_PIO_PA2	// LCD Reset

#define AT91B_SW1           AT91C_PIO_PB24  // Switch 1
#define AT91B_SW2           AT91C_PIO_PB25  // Switch 2

#define AT91A_JS_LEFT	    AT91C_PIO_PA7	// Joystick	left
#define AT91A_JS_RIGHT	    AT91C_PIO_PA8	//			right
#define AT91A_JS_UP		    AT91C_PIO_PA9	//			up
#define AT91A_JS_DOWN	    AT91C_PIO_PA14	//			down
#define AT91A_JS_CENTER	    AT91C_PIO_PA15	//			center

#define AT91A_JS_ALL       AT91A_JS_LEFT | AT91A_JS_RIGHT | AT91A_JS_UP | AT91A_JS_DOWN | AT91A_JS_CENTER



#define AT91B_DBGU_BAUD_RATE	115200


#define AT91B_MAIN_OSC        18432000               // Main Oscillator MAINCK
#define AT91B_MCK             ((18432000*73/14)/2)   // Output PLL Clock (48 MHz)
#define AT91B_MASTER_CLOCK    AT91B_MCK

#endif /* OLIMEX_SAM7_EX256_H */
