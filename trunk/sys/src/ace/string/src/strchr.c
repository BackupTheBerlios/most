/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
strchr (const char *cs, int c)
{
    for (; *cs != (char)c; ++cs)
        if (*cs == '\0')
            return NULL;
    return (char *)cs;
}
