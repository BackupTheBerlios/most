/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern int
ACE_strncmp (const char *cs, const char *ct, ACE_size_t n)
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
