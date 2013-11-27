/*
 * esc.h
 *
 *  Created on: 20.11.2013
 *      Author: maik
 *
 * This header is for the allowed escape sequences !
 * Only esc seq supported by the /dev/arch/ibmpc/console are allowed!
 * Folgende Farbcodes werden bei und ESC[#m oder ESC[#;#m unterstuetzt:
 *
 * Um VT100 compatible zu bleiben wird auf Farben verzichtet!!!!
 * Und nur NORMAL und FETT sind erlaubt!!!
 *
 *(nicht gültig:  es sollen aber nur Schwarz, Weiss, Rot und Gruen verwendet werden!!!
 * Da nur Rot und Gruen auf Weiss und Schwarz sichtbar ist!!!)
 *
 *
 * - Attribute:
 *  0 - Normale Schrift
 *  1 - Fett
 *  5 - Blinken
 * - Vordergrundfarben:
 *  30 - Schwarz
 *  31 - Rot
 *  32 - Gruen
 *  33 - Gelb
 *  34 - Blau
 *  35 - Magenta
 *  36 - Cyan
 *  37 - Weiss
 * - Hintergrundfarben:
 *  40 - Schwarz
 *  41 - Rot
 *  42 - Gruen
 *  43 - Gelb
 *  44 - Blau
 *  45 - Magenta
 *  46 - Cyan
 *  47 - Weiss
 *
 * Es koennen nur 1 oder 2 Farbcodes pro Escape-Sequenz angewendet werden.
 */

#ifndef ACE_ESC_H_
#define ACE_ESC_H_

/** Escape codes for terminal control
 *
 * This is a very limited subset of VT100 escape codes
 *
 * To be compatible with different terminal emulations
 * please use only escape codes which are defined here!
 *
 * The codes must also be a subset of ANSI escape codes
 * so that both VT100 and ANSI emulation can be used!
 *
 * Supported terminal emulation programs shall be:
 *
 * 1) The pc console!!!
 * 2) Minicom
 * 3) PuTTY
 * 4) Seyon
 *
 * Don't use colors so that the text is readable on black and white background!
 * Only use normal and bright font as this is a subset of PC console, VT100 and ANSI!
 */


#include <ace/stddef.h>

#define ACE_ESC   "\033"
#define ACE_CSI   ACE_ESC"["

#define ACE_ESC_SCROLL_DOWN          ACE_ESC"D"       //Scroll display down one line

#define ACE_ESC_CLEAR_SCREEN         ACE_CSI"2J"      //Bildschirm leeren
#define ACE_ESC_DEL_TO_END           ACE_CSI"K"       //Zeile von der Cursor-Position zum Ende leeren
#define ACE_ESC_NORMAL               ACE_CSI"0m"      //Normale Schrift (ausschalten von Fett und Blink
#define ACE_ESC_BRIGHT               ACE_CSI"1m"      //Fette Schrift
#define ACE_ESC_SAVE                 ACE_CSI"s"       //Cursor-Position speichern
#define ACE_ESC_UNSAVE               ACE_CSI"u"       //Cursor-Position wiederherstellen
#define ACE_ESC_REQUEST_CURSOR       ACE_CSI"6n"      //Request cursor position
#define ACE_ESC_LINE_WRAP_ON         ACE_CSI"7h"      //Enable line wrap
#define ACE_ESC_LINE_WRAP_OFF        ACE_CSI"7l"      //Disable line wrap

extern int ACE_esc_request_cursor(int *y, int *x, ACE_bool_t *valid);

extern void ACE_esc_set_cursor(int y, int x);

extern void ACE_esc_cursor_up(int n);

extern void ACE_esc_cursor_down(int n);

extern void ACE_esc_cursor_right(int n);

extern void ACE_esc_cursor_left(int n);

#endif /* ESC_H_ */
