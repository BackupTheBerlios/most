/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_CTYPE_H
#define ACE_CTYPE_H

/** @addtogroup ace
 *
 * @{
 */

/** @defgroup ctype ctype.h
 *
 * Standard character definitions.
 *
 * @{
 */

/*------------- Definitions ------------------------------------------*/

#define _ACE_U	0x01            /* upper */
#define _ACE_L	0x02            /* lower */
#define _ACE_D	0x04            /* digit */
#define _ACE_C	0x08            /* cntrl */
#define _ACE_P	0x10            /* punct */
#define _ACE_S	0x20            /* white space (space/lf/tab) */
#define _ACE_X	0x40            /* hex digit */
#define _ACE_SP	0x80            /* hard space (0x20) */

extern const unsigned char _ACE_ctype[];

/** Isalpha(c) or isdigit(c).
 */
#define ACE_isalnum(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_U|_ACE_L|_ACE_D))

/** Isupper(c) or islower(c).
 */
#define ACE_isalpha(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_U|_ACE_L))

/** Control character.
 */
#define ACE_iscntrl(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_C))

/** Decimal digit.
 */
#define ACE_isdigit(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_D))

/** Printable character, without space.
 */
#define ACE_isgraph(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_P|_ACE_U|_ACE_L|_ACE_D))

/** Lowercase letter.
 */
#define ACE_islower(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_L))

/** Printable character, with space.
 */
#define ACE_isprint(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_P|_ACE_U|_ACE_L|_ACE_D|_ACE_SP))

/** Printable character, without space, letter or digit.
 */
#define ACE_ispunct(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_P))

/** Space, formfeed \\f, linefeed \\n, cariage return \\r,
 *  tabulator \\t, vertical tabulator \\v.
 */
#define ACE_isspace(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_S))

/** Upper case letter.
 */
#define ACE_isupper(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_U))

/** Hexadecimale digit
 */
#define ACE_isxdigit(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_D|_ACE_X))

/** Convert c to lower case letter.
 */
extern char ACE_tolower(char c);

/** Convert c to upper case letter.
 */
extern char ACE_toupper(char c);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
