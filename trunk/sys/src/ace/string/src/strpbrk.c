/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
strpbrk (const char *cs, const char *ct)
{
    const char *p1;
    const char *p2;

    for (p1 = cs; *p1 != '\0'; ++p1)
    {
        for (p2 = ct; *p2 != '\0'; ++p2)
        {
            if (*p1 == *p2)
                return (char *)p1;
        }
    }
    return NULL;
}
