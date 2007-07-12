/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>

#include "uso/log.h"
#include "uso/mutex.h"

#define LOG_BUFFER_SIZE 0x100

/*------------- Implementation ------------------------------------------*/

static FILE *out = NULL;
static char buf[LOG_BUFFER_SIZE];
static enum USO_log_level log_level = USO_LL_PANIC;
static USO_mutex_t ll_sync;
static USO_mutex_t buf_sync;

static void
print_ll ()
{
    char *level;
    switch (log_level)
    {
    case USO_LL_PANIC:
        level = "PANIC";
        break;
    case USO_LL_ERROR:
        level = "ERROR";
        break;
    case USO_LL_WARNING:
        level = "WARNING";
        break;
    case USO_LL_INFO:
        level = "INFO";
        break;
    case USO_LL_DEBUG:
        level = "DEBUG";
        break;
    case USO_LL_PROTOCOL:
        level = "PROTOCOL";
        break;
    default:
        level = "WRONG";
        break;
    }
  	printf ("LOG LEVEL = %s\n", level);
}

extern void
USO_log_inc (void)
{
    USO_lock(&ll_sync);
    if (log_level < USO_LL_PROTOCOL)
        log_level++;
    print_ll ();
    USO_unlock(&ll_sync);
}

extern void
USO_log_dec (void)
{
    USO_lock(&ll_sync);
    if (log_level > USO_LL_PANIC)
        log_level--;
    print_ll ();
    USO_unlock(&ll_sync);
}

extern void
USO_log_show (void)
{
    USO_lock(&ll_sync);
    print_ll ();
    USO_unlock(&ll_sync);
}

extern void
USO_log_init (FILE * ios, enum USO_log_level level)
{
    out = ios;
	USO_mutex_init(&ll_sync);
	USO_mutex_init(&buf_sync);
    if (level >= USO_LL_PANIC && level <= USO_LL_PROTOCOL)
    {
        log_level = level;
    }
}

extern void
USO_kputs (enum USO_log_level level, char *string)
{
    if (level > log_level) { return; }
    fputs (out, string);
}

extern void
USO_kprintf (enum USO_log_level level, const char *fmt, ...)
{
    int len;
    va_list_t args;
    if (level > log_level) {return;}
    USO_lock(&buf_sync);
    va_start (args, fmt);
    len = vsprintf (buf, fmt, args);
    va_end (args);
	fwrite (out, buf, len);
    USO_unlock(&buf_sync);
}

extern void
USO_debug (const char *fmt, ...)
{
    int len;
    va_list_t args;
    if (USO_LL_DEBUG > log_level) {return;}
    USO_lock(&buf_sync);
    va_start (args, fmt);
    len = vsprintf (buf, fmt, args);
    va_end (args);
    fwrite (out, buf, len);
    USO_unlock(&buf_sync);
}

/*------------------------------------------------------------------------*/
