/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"
#include "ace/ctype.h"

extern int
atox (char c)
{
    int x;
    if (isdigit (c))
    {
        x = c - '0';
    }
    else if (isxdigit (c))
    {
        if (isupper (c))
        {
            x = 10 + c - 'A';
        }
        else if (islower (c))
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
atoxc (char* s)
{
    int x, x1, x2;
    x1 = atox (*s);
    x2 = atox (*(s+1));
    if ((x1 >= 0) && (x2 >= 0)) {
        x = (x1 << 4) | x2;
    } else {
        x = - 1;
    }
    return x;
}
