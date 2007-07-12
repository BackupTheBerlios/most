/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdio.h"

extern int
sprintf (char *buf, const char *fmt, ...)
{
    va_list_t args;
    int i;

    va_start (args, fmt);
    i = vsprintf (buf, fmt, args);
    va_end (args);

    return (i);
}
