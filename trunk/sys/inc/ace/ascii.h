/*
 * ascii.h
 *
 *  Created on: 18.11.2013
 *      Author: maik
 */

#ifndef ACE_ASCII_H_
#define ACE_ASCII_H_

#define ACE_ASCII_NUL     ((char)0x00)     /**< null */
#define ACE_ASCII_SOH     ((char)0x01)     /**< start of heading */
#define ACE_ASCII_STX     ((char)0x02)     /**< start of text */
#define ACE_ASCII_ETX     ((char)0x03)     /**< end of text */
#define ACE_ASCII_EOT     ((char)0x04)     /**< end of transmission */
#define ACE_ASCII_ENQ     ((char)0x05)     /**< enquiry */
#define ACE_ASCII_ACK     ((char)0x06)     /**< acknowledge */
#define ACE_ASCII_BELL    ((char)0x07)     /**< bell */
#define ACE_ASCII_BS      ((char)0x08)     /**< backspace */
#define ACE_ASCII_TAB     ((char)0x09)     /**< horizontal tab */
#define ACE_ASCII_LF      ((char)0x0A)     /**< line feed, NL new line */
#define ACE_ASCII_VT      ((char)0x0B)     /**< vertical tab */
#define ACE_ASCII_FF      ((char)0x0C)     /**< form feed, NP new page */
#define ACE_ASCII_CR      ((char)0x0D)     /**< carriage return */
#define ACE_ASCII_SO      ((char)0x0E)     /**< shift out */
#define ACE_ASCII_SI      ((char)0x0F)     /**< shift in */
#define ACE_ASCII_DLE     ((char)0x10)     /**< data link escape */
#define ACE_ASCII_DC1     ((char)0x11)     /**< device control 1, XON Ausgabe fortsetzen */
#define ACE_ASCII_DC2     ((char)0x12)     /**< device control 2, Reset */
#define ACE_ASCII_DC3     ((char)0x13)     /**< device control 3, XOFF Ausgabe anhalten */
#define ACE_ASCII_DC4     ((char)0x14)     /**< device control 4 */
#define ACE_ASCII_NAK     ((char)0x15)     /**< negative acknowledge */
#define ACE_ASCII_SYN     ((char)0x16)     /**< synchronous idle */
#define ACE_ASCII_ETB     ((char)0x17)     /**< end of trans. block */
#define ACE_ASCII_CAN     ((char)0x18)     /**< cancel */
#define ACE_ASCII_EM      ((char)0x19)     /**< end of medium */
#define ACE_ASCII_SUB     ((char)0x1A)     /**< substitute */
#define ACE_ASCII_ESC     ((char)0x1B)     /**< escape */
#define ACE_ASCII_FS      ((char)0x1C)     /**< file separator */
#define ACE_ASCII_GS      ((char)0x1D)     /**< group separator */
#define ACE_ASCII_RS      ((char)0x1E)     /**< record separator */
#define ACE_ASCII_US      ((char)0x1F)     /**< unit separator */
#define ACE_ASCII_Space   ((char)0x20)     /**< space */
#define ACE_ASCII_DEL     ((char)0x7F)     /**< delete */

#define ACE_CTRL_A     ACE_ASCII_SOH       /**< 0x01 */
#define ACE_CTRL_B     ACE_ASCII_STX       /**< 0x02 */
#define ACE_CTRL_C     ACE_ASCII_ETX       /**< 0x03 */
#define ACE_CTRL_D     ACE_ASCII_EOT       /**< 0x04 */
#define ACE_CTRL_E     ACE_ASCII_ENQ       /**< 0x05 */
#define ACE_CTRL_F     ACE_ASCII_ACK       /**< 0x06 */
#define ACE_CTRL_G     ACE_ASCII_BELL      /**< 0x07 */
#define ACE_CTRL_H     ACE_ASCII_BS        /**< 0x08 */
#define ACE_CTRL_I     ACE_ASCII_TAB       /**< 0x09 */
#define ACE_CTRL_J     ACE_ASCII_LF        /**< 0x0A */
#define ACE_CTRL_K     ACE_ASCII_VT        /**< 0x0B */
#define ACE_CTRL_L     ACE_ASCII_FF        /**< 0x0C */
#define ACE_CTRL_M     ACE_ASCII_CR        /**< 0x0D */
#define ACE_CTRL_N     ACE_ASCII_SO        /**< 0x0E */
#define ACE_CTRL_O     ACE_ASCII_SI        /**< 0x0F */
#define ACE_CTRL_P     ACE_ASCII_DLE       /**< 0x10 */
#define ACE_CTRL_Q     ACE_ASCII_DC1       /**< 0x11 */
#define ACE_CTRL_R     ACE_ASCII_DC2       /**< 0x12 */
#define ACE_CTRL_S     ACE_ASCII_DC3       /**< 0x13 */
#define ACE_CTRL_T     ACE_ASCII_DC4       /**< 0x14 */
#define ACE_CTRL_U     ACE_ASCII_NAK       /**< 0x15 */
#define ACE_CTRL_V     ACE_ASCII_SYN       /**< 0x16 */
#define ACE_CTRL_W     ACE_ASCII_ETB       /**< 0x17 */
#define ACE_CTRL_X     ACE_ASCII_CAN       /**< 0x18 */
#define ACE_CTRL_Y     ACE_ASCII_EM        /**< 0x19 */
#define ACE_CTRL_Z     ACE_ASCII_SUB       /**< 0x1A */


#endif /* ACE_ASCII_H_ */
