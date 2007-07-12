/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern int
memcmp (const void *cs, const void *ct, size_t n)
{
    const unsigned char *p1, *p2;
    signed char ret = 0;

    for (p1 = cs, p2 = ct; 0 < n; ++p1, ++p2, n--)
        if ((ret = *p1 - *p2) != 0)
            break;
    return ret;
}
