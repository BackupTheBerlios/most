/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdio.h>

#include "uso/scheduler.h"
#include "uso/thread.h"
#include "uso/buf_pool.h"
#include "mfs/stream.h"

/*------------- Implementation ------------------------------------------*/

#define IOS_BUF_COUNT 3
#define IOS_BUF_SIZE 0x100
static USO_buf_pool_t ios_buf_pool;
static char ios_bufs[IOS_BUF_COUNT][IOS_BUF_SIZE];

extern void
ACE_stdio_init (void)
{
    USO_buf_pool_init (&ios_buf_pool, ios_bufs, IOS_BUF_COUNT, IOS_BUF_SIZE);
}


extern ACE_size_t
ACE_fread (ACE_FILE * in, char *buf, ACE_size_t len){
    if (in == NULL) {return 0;}
	return MFS_read(in, buf, len);
}

extern ACE_size_t
ACE_fwrite (ACE_FILE * out, char *buf, ACE_size_t len){
    if (out == NULL) {return 0;}
	return MFS_write(out, buf, len);
}

extern int
ACE_fgetc (ACE_FILE * in)
{
	char c;
	if (ACE_fread(in, &c, sizeof(c)) >= sizeof(c)){
		 return (unsigned char)c;
	}
    return ACE_EOF;
}

extern int
ACE_fputc (ACE_FILE * out, char c)
{
	int ret = ACE_fwrite (out, &c, sizeof(c));
    if (ret < sizeof(c)) {ret = ACE_EOF;}
    return ret;
}

extern int
ACE_fputs (ACE_FILE * out, char *string)
{
    int len = ACE_strlen(string);
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
    buf = USO_buf_alloc (&ios_buf_pool);
    ACE_va_start (args, fmt);
    len = ACE_vsprintf (buf, fmt, args);
    ACE_va_end (args);
    ret = ACE_fwrite (out, buf, len);
    USO_buf_free (&ios_buf_pool, buf);
    return (ret < len) ? ACE_EOF : ret;
}


extern ACE_size_t
ACE_read (char *buf, ACE_size_t len)
{
    return ACE_fread (USO_current()->in, buf, len);
}

extern ACE_size_t
ACE_write (char *buf, ACE_size_t len)
{
    return ACE_fwrite (USO_current()->out, buf, len);
}

extern int
ACE_getc (void)
{
    return ACE_fgetc (USO_current()->in);
}

extern int
ACE_putc (char c)
{
    return ACE_fputc (USO_current()->out, c);
}

extern int
ACE_puts (char *string)
{
    return ACE_fputs (USO_current()->out, string);
}

extern int
ACE_printf (const char *fmt, ...)
{
    int len;
	int ret;
    ACE_va_list_t args;
    char *buf;
    buf = USO_buf_alloc (&ios_buf_pool);
    ACE_va_start (args, fmt);
    len = ACE_vsprintf (buf, fmt, args);
    ACE_va_end (args);
    ret = ACE_fwrite (USO_current()->out, buf, len);
    USO_buf_free (&ios_buf_pool, buf);
    return (ret < len) ? ACE_EOF : ret;
}

/*------------------------------------------------------------------------*/
