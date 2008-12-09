/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdio.h"

extern int
ACE_sprintf (char *buf, const char *fmt, ...)
{
    ACE_va_list_t args;
    int i;

    ACE_va_start (args, fmt);
    i = ACE_vsprintf (buf, fmt, args);
    ACE_va_end (args);

    return (i);
}
