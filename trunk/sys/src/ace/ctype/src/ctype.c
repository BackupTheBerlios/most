/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/ctype.h>


const unsigned char _ACE_ctype[] = {
    _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C,     /* 0-7 
                                                                         */
    _ACE_C, _ACE_C | _ACE_S, _ACE_C | _ACE_S, _ACE_C | _ACE_S, _ACE_C | _ACE_S, _ACE_C | _ACE_S, _ACE_C, _ACE_C,        /* 8-15 
                                                                                                                         */
    _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C,     /* 16-23 
                                                                         */
    _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C, _ACE_C,     /* 24-31 
                                                                         */
    _ACE_S | _ACE_SP, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,   /* 32-39 
                                                                                 */
    _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,     /* 40-47 
                                                                         */
    _ACE_D, _ACE_D, _ACE_D, _ACE_D, _ACE_D, _ACE_D, _ACE_D, _ACE_D,     /* 48-55 
                                                                         */
    _ACE_D, _ACE_D, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,     /* 56-63 
                                                                         */
    _ACE_P, _ACE_U | _ACE_X, _ACE_U | _ACE_X, _ACE_U | _ACE_X,
    _ACE_U | _ACE_X, _ACE_U | _ACE_X, _ACE_U | _ACE_X, _ACE_U,  /* 64-71 */
    _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U,     /* 72-79 
                                                                         */
    _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U,     /* 80-87 
                                                                         */
    _ACE_U, _ACE_U, _ACE_U, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,     /* 88-95 
                                                                         */
    _ACE_P, _ACE_L | _ACE_X, _ACE_L | _ACE_X, _ACE_L | _ACE_X,
    _ACE_L | _ACE_X, _ACE_L | _ACE_X, _ACE_L | _ACE_X, _ACE_L,  /* 96-103 */
    _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L,     /* 104-111 
                                                                         */
    _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L,     /* 112-119 
                                                                         */
    _ACE_L, _ACE_L, _ACE_L, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_C,     /* 120-127 
                                                                         */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 128-143 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 144-159 */
    _ACE_S | _ACE_SP, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,
    _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,     /* 160-175 
                                                                                 */
    _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,
    _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P, _ACE_P,     /* 176-191 
                                                                 */
    _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U,
    _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U,     /* 192-207 
                                                                 */
    _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_P, _ACE_U,
    _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_U, _ACE_L,     /* 208-223 
                                                                 */
    _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L,
    _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L,     /* 224-239 
                                                                 */
    _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_P, _ACE_L,
    _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L, _ACE_L
};                              /* 240-255 */


extern char
ACE_tolower(char c)
{
     return ACE_isupper(c) ? c - ('A'-'a') : c;
}

extern char
ACE_toupper(char c)
{
    return ACE_islower(c) ? c - ('a'-'A') : c;
}
