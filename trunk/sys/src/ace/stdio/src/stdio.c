/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdio.h>

#include <uso/thread.h>
#include <mfs/stream.h>
#include <uso/scheduler.h>


USO_buf_pool_t *ACE_ios_buf_pool;

extern void
ACE_stdio_init (USO_buf_pool_t *ios_buf_pool)
{
    ACE_ios_buf_pool = ios_buf_pool;
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
    unsigned char c;
    if (ACE_fread (in, (char*)&c, sizeof (c)) >= sizeof (c))
    {
        return (int)c;
    }
    return ACE_EOF;
}

extern int
ACE_fputc (ACE_FILE * out, int c)
{
    unsigned char t;
    if (c == ACE_EOF) return c;
    t = (unsigned char)c;
    if (ACE_fwrite (out, (char*)&t, sizeof (t)) >= sizeof (t)){
        return (int)t;
    }
    return ACE_EOF;
}

extern char*
ACE_fgets (ACE_FILE * in, char *string, ACE_size_t len)
{
    char *ret = string;
    if (len == 0 || string == NULL) return NULL;
    for (int n = 0; n < len - 1; ++n)
    {
        char c;
        if (ACE_fread (in, &c, sizeof (c)) >= sizeof (c))
        {
            *string++ = c;
            if (c == '\0') return ret;
            if (c == '\n') break;
        } else {
            break; /* EOF */
        }
    }
    *string = '\0';
    return ret;
}

extern int
ACE_fputs (ACE_FILE * out, char *string)
{
    int len = ACE_strlen (string);
    int ret = ACE_fwrite (out, string, len);
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
ACE_putc (int c)
{
    return ACE_fputc (USO_current ()->out, c);
}

extern char*
ACE_gets (char *string, ACE_size_t len)
{
    return ACE_fgets (USO_current ()->in, string, len);
}

extern int
ACE_puts (char *string)
{
    return ACE_fputs (USO_current ()->out, string);
}

