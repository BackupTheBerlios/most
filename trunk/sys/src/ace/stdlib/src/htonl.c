/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"

#ifdef ACE_BYTE_ORDER
#if ACE_BYTE_ORDER==ACE_LITTLE_ENDIAN

u32_t
htonl (u32_t n)
{
    return ((n & 0xff) << 24) |
        ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24);
}

#endif
#endif
