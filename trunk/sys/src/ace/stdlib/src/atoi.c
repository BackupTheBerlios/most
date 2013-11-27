/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"
#include "ace/ctype.h"

extern int
ACE_atoi (const char *s)
{
    int l;
    for (l = 0; (ACE_isdigit (*s)); l = (l * 10) + ((*s++) - '0'));
    return (l);
}

extern int
ACE_xtoi (char c)
{
    int x;
    if (ACE_isdigit (c))
    {
        x = c - '0';
    }
    else if (ACE_isxdigit (c))
    {
        if (ACE_isupper (c))
        {
            x = 10 + c - 'A';
        }
        else if (ACE_islower (c))
        {
            x = 10 + c - 'a';
        }
        else
        {
            x = -1;
        }
    }
    else
    {
        x = -1;
    }
    return x;
}

extern int
ACE_xxtoi (char *s)
{
    int x, x1, x2;
    x1 = ACE_xtoi (*s);
    x2 = ACE_xtoi (*(s + 1));
    if ((x1 >= 0) && (x2 >= 0))
    {
        x = (x1 << 4) | x2;
    }
    else
    {
        x = -1;
    }
    return x;
}
