/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
ACE_strncpy (char *s, const char *ct, ACE_size_t n)
{
    char *temp = s;

    while (n && ((*s++ = *ct++) != '\0'))
    {
        --n;
    }
    if (n > 0)
    {
        while (n--)
            *s++ = 0;
    }
    return temp;
}
