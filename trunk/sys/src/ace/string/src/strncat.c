/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
ACE_strncat (char *s, const char *ct, ACE_size_t n)
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
