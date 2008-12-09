/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern int
ACE_strcmp (const char *cs, const char *ct)
{
    int ret;

    while (1)
    {
        if ((ret = *cs - *ct++) != 0 || !*cs++)
            break;
    }
    return ret;
}
