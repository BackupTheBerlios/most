/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_CTYPE_H
#define ACE_CTYPE_H

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
extern char _ACE_ctmp;

/** isalpha(c) oder isdigit(c)
 */
#define ACE_isalnum(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_U|_ACE_L|_ACE_D))

/** isupper(c) oder islower(c)
 */
#define ACE_isalpha(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_U|_ACE_L))

/** Steuerzeichen
 */
#define ACE_iscntrl(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_C))

/** dezimale Ziffer
 */
#define ACE_isdigit(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_D))

/** sichtbares Zeichen, kein Leerzeichen
 */
#define ACE_isgraph(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_P|_ACE_U|_ACE_L|_ACE_D))

/** Kleinbuchstabe aber kein Umlaut
 */
#define ACE_islower(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_L))

/** sichtbares Zeichen, auch Leerzeichen
 */
#define ACE_isprint(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_P|_ACE_U|_ACE_L|_ACE_D|_ACE_SP))

/** sichtbares Zeichen, mit Ausnahme von Leerzeichen, Buchstabe oder Ziffer
 */
#define ACE_ispunct(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_P))

/** Leerzeichen, Seitenvorschub \\f, Zeilentrenner \\n, Wagenru"cklauf \\r,
 *  Tabulator \\t, Vertikal Tabulator \\v
 */
#define ACE_isspace(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_S))

/** Groszbuchstabe
 */
#define ACE_isupper(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_U))

/** hexadezimale Ziffer
 */
#define ACE_isxdigit(c) (_ACE_ctype[(unsigned char)(c)]&(_ACE_D|_ACE_X))

/** wandelt c in einen Kleinbuchstaben um
 */
#define ACE_tolower(c) (_ACE_ctmp=(c), isupper(_ACE_ctmp) ? _ACE_ctmp - ('A'-'a') : _ACE_ctmp)

/** wandelt c in einen Groszbuchstaben um
 */
#define ACE_toupper(c) (_ACE_ctmp=(c), islower(_ACE_ctmp) ? _ACE_ctmp - ('a'-'A') : _ACE_ctmp)

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
