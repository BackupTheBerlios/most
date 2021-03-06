/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
ACE_strrchr (const char *cs, int c)
{
    const char *p = cs + ACE_strlen (cs);
    do
    {
        if (*p == (char)c)
            return (char *)p;
    }
    while (--p >= cs);
    return NULL;
}
