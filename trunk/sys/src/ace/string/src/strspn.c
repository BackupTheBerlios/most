/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern size_t
strspn (const char *cs, const char *ct)
{
    const char *p1;
    const char *p2;
    size_t n = 0;

    for (p1 = cs; *p1 != '\0'; ++p1)
    {
        for (p2 = ct; *p2 != '\0'; ++p2)
        {
            if (*p1 == *p2)
                break;
        }
        if (*p2 == '\0')
            return n;
        ++n;
    }
    return n;
}
