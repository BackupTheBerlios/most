/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#include "net/netbuf.h"
#include "net/ethif.h"
#include "net/ip.h"
#include "net/udp.h"

#define NET_BUF_COUNT 20
#define NET_TRANSBUF_COUNT 10

static USO_buf_pool_t net_pool;
static NET_netbuf_t net_bufs[NET_BUF_COUNT];

static USO_buf_pool_t trans_pool;
struct trans_buf
{
	NET_netbuf_t net_buf;
    struct NET_eth_hdr eth_hdr;
    struct NET_ip_hdr ip_hdr;
    struct NET_udp_hdr udp_hdr;
    char space[64];
} _PACKED_;
static struct trans_buf trans_bufs[NET_TRANSBUF_COUNT];



extern void
NET_netbuf_init (void)
{
    USO_buf_pool_init (&net_pool, net_bufs,
                       NET_BUF_COUNT, sizeof (NET_netbuf_t));
    USO_buf_pool_init (&trans_pool, trans_bufs,
                       NET_TRANSBUF_COUNT, sizeof (struct trans_buf));
}

extern NET_netbuf_t *
NET_netbuf_alloc_trans (void)
{
    NET_netbuf_t *buf = USO_buf_alloc (&trans_pool);
    buf->type = NET_BUF_TRANS;
    buf->data = ((char*)buf) + sizeof (NET_netbuf_t);
    buf->size = trans_pool.buf_size - sizeof (NET_netbuf_t);
    buf->next = NULL;
    buf->index = ((char*)buf) + trans_pool.buf_size;
    buf->len = 0;
    return buf;
}

static void
NET_netbuf_alloc_help(NET_netbuf_t *buf, long size, char *data)
{
    buf->data = data;
    buf->size = size;
    buf->next = NULL;
    buf->index = buf->data;
    buf->len = buf->size;
}

extern NET_netbuf_t *
NET_netbuf_alloc_pool (USO_buf_pool_t * pool)
{
    NET_netbuf_t *buf = USO_buf_alloc (&net_pool);
    buf->type = NET_BUF_POOL;
	buf->pool = pool;
	NET_netbuf_alloc_help(buf, pool->buf_size, USO_buf_alloc (pool));
    return buf;
}

extern NET_netbuf_t *
NET_netbuf_alloc_ram (long size)
{
	char *data = malloc (size);
    if (data == NULL){ return NULL; }
    NET_netbuf_t *buf = USO_buf_alloc (&net_pool);
    buf->type = NET_BUF_RAM;
    buf->pool = NULL;
	NET_netbuf_alloc_help(buf, size, data);
    return buf;
}

extern NET_netbuf_t *
NET_netbuf_alloc_rom (long size, char *data)
{
    NET_netbuf_t *buf = USO_buf_alloc (&net_pool);
    buf->type = NET_BUF_ROM;
    buf->pool = NULL;
	NET_netbuf_alloc_help(buf, size, data);
    return buf;
}

extern void
NET_netbuf_free (NET_netbuf_t * buf)
{
    NET_netbuf_t *next;
    while (buf != NULL)
    {
        next = buf->next;
        switch (buf->type)
        {
        	case NET_BUF_RAM:
            	free (buf->data);
            	break;
            case NET_BUF_POOL:
		        USO_buf_free (buf->pool, buf->data);
		        break;
			default:
				break;
        }
        switch (buf->type)
        {
        	case NET_BUF_TRANS:
		        USO_buf_free (&trans_pool, buf);
		        break;
			default:
		        USO_buf_free (&net_pool, buf);
		        break;
        }
        buf = next;
    }
}

extern NET_netbuf_t *
NET_netbuf_chain (NET_netbuf_t * a, NET_netbuf_t * b)
{
    NET_netbuf_t *next;
    for (next = a; next->next != NULL; next = next->next);
    next->next = b;
    return a;
}

extern bool_t
NET_netbuf_index_inc (NET_netbuf_t * buf, long inc)
{
	bool_t ok = FALSE;
	if ( (buf->index + inc) >= buf->data &&
	     (buf->index + inc) <= (buf->data + buf->size) ){
    	buf->index += inc;
    	buf->len -= inc;
    	ok = TRUE;
	}
	return ok;
}

extern bool_t
NET_netbuf_len_adjust (NET_netbuf_t * buf, unsigned long len)
{
	bool_t ok = FALSE;
    if (len <= buf->len)
    {
        buf->len = len;
        ok = TRUE;
    }
    return ok;
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
