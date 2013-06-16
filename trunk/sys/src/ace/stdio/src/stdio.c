/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdio.h>

#include <uso/scheduler.h>
#include <uso/thread.h>
#include <mfs/stream.h>

/*------------- Implementation ------------------------------------------*/

static USO_buf_pool_t *buf_pool;

extern void
ACE_stdio_init (USO_buf_pool_t *ios_buf_pool)
{
    buf_pool = ios_buf_pool;
}


extern ACE_size_t
ACE_fread (ACE_FILE * in, char *buf, ACE_size_t len)
{
    if (in != NULL && in->type == MFS_STREAM)
    {
        return MFS_read ((MFS_stream_t *)in, buf, len);
    }
    return 0;
}

extern ACE_size_t
ACE_fwrite (ACE_FILE * out, char *buf, ACE_size_t len)
{
    if (out != NULL && out->type == MFS_STREAM)
    {
        return MFS_write ((MFS_stream_t *)out, buf, len);
    }
    return 0;
}

extern int
ACE_fgetc (ACE_FILE * in)
{
    char c;
    if (ACE_fread (in, &c, sizeof (c)) >= sizeof (c))
    {
        return (unsigned char)c;
    }
    return ACE_EOF;
}

extern int
ACE_fputc (ACE_FILE * out, char c)
{
    int ret = ACE_fwrite (out, &c, sizeof (c));
    return ret;
}

extern int
ACE_fputs (ACE_FILE * out, char *string)
{
    int len = ACE_strlen (string);
    int ret = ACE_fwrite (out, string, len);
    return (ret < len) ? ACE_EOF : ret;
}

extern int
ACE_fprintf (ACE_FILE * out, const char *fmt, ...)
{
    int len;
    int ret;
    ACE_va_list_t args;
    char *buf;
    buf = USO_buf_alloc (buf_pool);
    ACE_va_start (args, fmt);
    len = ACE_vsprintf (buf, fmt, args);
    ACE_va_end (args);
    ret = ACE_fwrite (out, buf, len);
    USO_buf_free (buf_pool, buf);
    return (ret < len) ? ACE_EOF : ret;
}


extern ACE_size_t
ACE_read (char *buf, ACE_size_t len)
{
    return ACE_fread (USO_current ()->in, buf, len);
}

extern ACE_size_t
ACE_write (char *buf, ACE_size_t len)
{
    return ACE_fwrite (USO_current ()->out, buf, len);
}

extern int
ACE_getc (void)
{
    return ACE_fgetc (USO_current ()->in);
}

extern int
ACE_putc (char c)
{
    return ACE_fputc (USO_current ()->out, c);
}

extern int
ACE_puts (char *string)
{
    return ACE_fputs (USO_current ()->out, string);
}

extern int
ACE_printf (const char *fmt, ...)
{
    int len;
    int ret;
    ACE_va_list_t args;
    char *buf;
    buf = USO_buf_alloc (buf_pool);
    ACE_va_start (args, fmt);
    len = ACE_vsprintf (buf, fmt, args);
    ACE_va_end (args);
    ret = ACE_fwrite (USO_current ()->out, buf, len);
    USO_buf_free (buf_pool, buf);
    return (ret < len) ? ACE_EOF : ret;
}

/*------------------------------------------------------------------------*/
