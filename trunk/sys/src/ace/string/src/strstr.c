/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
ACE_strstr (const char *cs, const char *ct)
{
    int l1, l2;

    l2 = ACE_strlen (ct);
    if (!l2)
        return (char *)cs;
    l1 = ACE_strlen (cs);
    while (l1 >= l2)
    {
        l1--;
        if (!memcmp (cs, ct, l2))
            return (char *)cs;
        cs++;
    }
    return NULL;
}
