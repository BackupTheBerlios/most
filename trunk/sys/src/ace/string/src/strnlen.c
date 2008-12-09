/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern ACE_size_t
ACE_strnlen (const char *cs, ACE_size_t n)
{
    const char *p;

    for (p = cs; n-- && *p != '\0'; ++p);
    return (p - cs);
}
