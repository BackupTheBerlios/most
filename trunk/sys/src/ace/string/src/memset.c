/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern void *
memset (void *s, int c, size_t n)
{
    char *xs = (char *)s;

    while (n--)
        *xs++ = c;
    return s;
}
