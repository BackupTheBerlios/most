/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern int
strncmp (const char *cs, const char *ct, size_t n)
{
    int ret = 0;

    while (n)
    {
        if ((ret = *cs - *ct++) != 0 || !*cs++)
            break;
        n--;
    }
    return ret;
}
