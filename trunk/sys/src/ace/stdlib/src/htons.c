/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"

#ifdef ACE_BYTE_ORDER
#if ACE_BYTE_ORDER == ACE_LITTLE_ENDIAN

extern ACE_u16_t
ACE_htons (ACE_u16_t n)
{
    return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

#endif
#endif
