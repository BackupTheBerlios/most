/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#if defined MOST_ENDIAN_LITTLE

extern ACE_u16_t
ACE_htons (ACE_u16_t n)
{
    return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

#endif
