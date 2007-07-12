/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

static char *___strtok = NULL;

extern char *
strtok (char *s, const char *ct)
{
    char *begin, *end;

    begin = s ? s : ___strtok;
    if (!begin)
    {
        return (NULL);
    }
    begin += strspn (begin, ct);
    if (*begin == '\0')
    {
        ___strtok = NULL;
        return NULL;
    }
    end = strpbrk (begin, ct);
    if (end && *end != '\0')
        *end++ = '\0';
    ___strtok = end;
    return begin;
}
