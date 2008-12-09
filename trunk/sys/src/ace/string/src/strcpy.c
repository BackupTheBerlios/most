/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/string.h"

extern char *
ACE_strcpy (char *s, const char *ct)
{
    char *temp = s;

    while ((*s++ = *ct++) != '\0');
    return temp;
}
