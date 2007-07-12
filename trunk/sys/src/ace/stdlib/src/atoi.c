/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdlib.h"
#include "ace/ctype.h"

extern int
atoi (const char *s)
{
    int l;
    for (l = 0; (isdigit (*s)); l = (l * 10) + ((*s++) - '0'));
    return (l);
}
