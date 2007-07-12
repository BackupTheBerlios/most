/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"
#include "ace/ctype.h"

extern long
atol (const char *s)
{
    long l;
    for (l = 0; (isdigit (*s)); l = (l * 10) + ((*s++) - '0'));
    return (l);
}
