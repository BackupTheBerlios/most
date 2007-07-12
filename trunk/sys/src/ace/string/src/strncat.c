/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
strncat (char *s, const char *ct, size_t n)
{
    char *temp = s;

    if (n)
    {
        while (*s)
            s++;
        while ((*s++ = *ct++))
        {
            if (--n == 0)
            {
                *s = '\0';
                break;
            }
        }
    }
    return temp;
}
