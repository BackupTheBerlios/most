/*
 * fprintf.c
 *
 *  Created on: 24.11.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdarg.h>

extern int
ACE_fprintf (ACE_FILE * out, const char *fmt, ...)
{
    int len;
    int ret;
    ACE_va_list_t args;
    char *buf;
    buf = USO_buf_alloc (ACE_ios_buf_pool);
    ACE_va_start (args, fmt);
    len = ACE_vsprintf (buf, fmt, args);
    ACE_va_end (args);
    ret = ACE_fwrite (out, buf, len);
    USO_buf_free (ACE_ios_buf_pool, buf);
    return (ret < len) ? ACE_EOF : ret;
}

