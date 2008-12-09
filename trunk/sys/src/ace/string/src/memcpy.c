/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern void *
memcpy (void *s, const void *ct, ACE_size_t n)
{
    char *p1 = (char *)s, *p2 = (char *)ct;

    while (n--)
        *p1++ = *p2++;
    return s;
}
