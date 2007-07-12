/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern size_t
strnlen (const char *cs, size_t n)
{
    const char *p;

    for (p = cs; n-- && *p != '\0'; ++p);
    return (p - cs);
}
