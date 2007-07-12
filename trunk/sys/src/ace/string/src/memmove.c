/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern void *
memmove (void *s, const void *ct, size_t n)
{
    char *p1, *p2;

    if (s <= ct)
    {
        p1 = (char *)s;
        p2 = (char *)ct;
        while (n--)
            *p1++ = *p2++;
    }
    else
    {
        p1 = (char *)s + n;
        p2 = (char *)ct + n;
        while (n--)
            *--p1 = *--p2;
    }
    return s;
}
