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


extern size_t
fread (FILE * in, char *buf, size_t len){
    if (in == NULL) {return 0;}
	return MFS_read(in, buf, len);
}

extern size_t
fwrite (FILE * out, char *buf, size_t len){
    if (out == NULL) {return 0;}
	return MFS_write(out, buf, len);
}

extern int
fgetc (FILE * in)
{
	char c;
	if (fread(in, &c, sizeof(c)) >= sizeof(c)){
		 return (unsigned char)c;
	}
    return EOF;
}

extern int
fputc (FILE * out, char c)
{
	int ret = fwrite (out, &c, sizeof(c));
    if (ret < sizeof(c)) {ret = EOF;}
    return ret;
}

extern int
fputs (FILE * out, char *string)
{
    int len = strlen(string);
	int ret = fwrite (out, string, len);
    return (ret < len) ? EOF : ret;
}

extern int
fprintf (FILE * out, const char *fmt, ...)
{
    int len;
	int ret;
    va_list_t args;
    char *buf;
    buf = USO_buf_alloc (&ios_buf_pool);
    va_start (args, fmt);
    len = vsprintf (buf, fmt, args);
    va_end (args);
    ret = fwrite (out, buf, len);
    USO_buf_free (&ios_buf_pool, buf);
    return (ret < len) ? EOF : ret;
}


extern size_t
read (char *buf, size_t len)
{
    return fread (USO_current()->in, buf, len);
}

extern size_t
write (char *buf, size_t len)
{
    return fwrite (USO_current()->out, buf, len);
}

extern int
getc (void)
{
    return fgetc (USO_current()->in);
}

extern int
putc (char c)
{
    return fputc (USO_current()->out, c);
}

extern int
puts (char *string)
{
    return fputs (USO_current()->out, string);
}

extern int
printf (const char *fmt, ...)
{
    int len;
	int ret;
    va_list_t args;
    char *buf;
    buf = USO_buf_alloc (&ios_buf_pool);
    va_start (args, fmt);
    len = vsprintf (buf, fmt, args);
    va_end (args);
    ret = fwrite (USO_current()->out, buf, len);
    USO_buf_free (&ios_buf_pool, buf);
    return (ret < len) ? EOF : ret;
}

/*------------------------------------------------------------------------*/
