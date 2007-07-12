/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#include "net/netbuf.h"

extern NET_netbuf_t *
NET_netbuf_alloc (USO_buf_pool_t * pool, long size, char *data)
{
    NET_netbuf_t *buf = USO_buf_alloc (pool);
    if (size == 0)
    {
        buf->size = pool->buf_size - sizeof (NET_netbuf_t);
        buf->data = ((char *)buf) + sizeof (NET_netbuf_t);
        buf->type = NET_BUF_POOL;
    }
    else
    {
        if (data == NULL)
        {
            data = malloc (size);
            if (data == NULL)
            {
                USO_buf_free (pool, buf);
                return NULL;
            }
            buf->type = NET_BUF_RAM;
        }
        else
        {
            buf->type = NET_BUF_ROM;
        }
        buf->size = size;
        buf->data = data;
    }
    buf->pool = pool;
    buf->next = NULL;
    buf->index = buf->data;
    buf->len = buf->size;
    return buf;
}

extern void
NET_netbuf_free (NET_netbuf_t * buf)
{
    NET_netbuf_t *next;
    while (buf != NULL)
    {
        next = buf->next;
        if (buf->type == NET_BUF_RAM)
        {
            free (buf->data);
        }
        USO_buf_free (buf->pool, buf);
        buf = next;
    }
}

extern NET_netbuf_t *
NET_netbuf_chain (NET_netbuf_t * buf1, NET_netbuf_t * buf2)
{
    NET_netbuf_t *next;
    for (next = buf1; next->next != NULL; next = next->next);
    next->next = buf2;
    return buf1;
}

extern void
NET_netbuf_index_inc (NET_netbuf_t * buf, long inc)
{
    buf->index += inc;
    buf->len -= inc;
}

extern bool_t
NET_netbuf_len_adjust (NET_netbuf_t * buf, unsigned long len)
{
    if (len <= buf->len)
    {
        buf->len = len;
        return TRUE;
    }
    return FALSE;
}

extern long
NET_netbuf_tot_len (NET_netbuf_t * buf)
{
    long tot_len;
    for (tot_len = 0; buf != NULL; buf = buf->next)
    {
        tot_len += buf->len;
    }
    return tot_len;
}
