/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"

#if defined MOST_ENDIAN_LITTLE

ACE_u32_t
ACE_htonl (ACE_u32_t n)
{
    return ((n & 0xff) << 24) |
        ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24);
}

#endif
