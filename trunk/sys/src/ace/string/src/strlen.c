/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern size_t
strlen (const char *cs)
{
    const char *p;

    for (p = cs; *p != '\0'; ++p);
    return (p - cs);
}
