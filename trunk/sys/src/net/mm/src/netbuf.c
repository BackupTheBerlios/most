/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#include "net/netbuf.h"
#include "net/ethif.h"
#include "net/ip.h"
#include "net/udp.h"
#include "net/debug.h"

#define NET_BUF_COUNT 20
#define NET_TRANSBUF_COUNT 10

struct NET_netbuf
{
    USO_node_t node;
    NET_netbuf_t *next;
    char * data;
    long size;
    char *index;
    long len;
    USO_buf_pool_t *pool;
    enum NET_buf_type type;
};

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
};
static struct trans_buf trans_bufs[NET_TRANSBUF_COUNT];

extern void
NET_netbuf_init (void)
{
    USO_buf_pool_init (&net_pool, net_bufs,
                       NET_BUF_COUNT, sizeof (NET_netbuf_t));
    USO_buf_pool_init (&trans_pool, trans_bufs,
                       NET_TRANSBUF_COUNT, sizeof (struct trans_buf));
}

static void
NET_netbuf_alloc_help(NET_netbuf_t *buf, char *data, long size)
{
    buf->data = data;
    buf->size = size;
    buf->next = NULL;
    buf->index = buf->data;
    buf->len = buf->size;
}

extern NET_netbuf_t *
NET_netbuf_alloc_trans (void)
{
	char *data;
	ACE_size_t size;
    NET_netbuf_t *buf = USO_buf_alloc (&trans_pool);
    buf->type = NET_BUF_TRANS;
    buf->pool = NULL;
	data = ((char*)buf) + sizeof (NET_netbuf_t);
	size = trans_pool.buf_size - sizeof (NET_netbuf_t);
	NET_netbuf_alloc_help(buf, data, size);
	NET_netbuf_index_inc (buf, size);
    return buf;
}

extern NET_netbuf_t *
NET_netbuf_alloc_pool (USO_buf_pool_t * pool)
{
    NET_netbuf_t *buf = USO_buf_alloc (&net_pool);
    buf->type = NET_BUF_POOL;
	buf->pool = pool;
	NET_netbuf_alloc_help(buf, USO_buf_alloc (pool), pool->buf_size);
    return buf;
}

extern NET_netbuf_t *
NET_netbuf_alloc_ram (long size)
{
	char *data = ACE_malloc (size);
    if (data == NULL){ return NULL; }
    NET_netbuf_t *buf = USO_buf_alloc (&net_pool);
    buf->type = NET_BUF_RAM;
    buf->pool = NULL;
	NET_netbuf_alloc_help(buf, data, size);
    return buf;
}

extern NET_netbuf_t *
NET_netbuf_alloc_rom (char *data, long size)
{
    NET_netbuf_t *buf = USO_buf_alloc (&net_pool);
    buf->type = NET_BUF_ROM;
    buf->pool = NULL;
	NET_netbuf_alloc_help(buf, data, size);
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
            	ACE_free (buf->data);
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
			case NET_BUF_ROM:
			case NET_BUF_RAM:
			case NET_BUF_POOL:
		        USO_buf_free (&net_pool, buf);
		        break;
			case NET_BUF_FREE:
			    USO_kprintf (USO_LL_ERROR, "netbuf free\n");
				break;
			default:
			    USO_kprintf (USO_LL_ERROR, "netbuf inval\n");
		        break;
        }
		buf->type = NET_BUF_FREE;
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

extern NET_netbuf_t *
NET_netbuf_next (NET_netbuf_t * buf)
{
	if (buf != NULL){
		return buf->next;
	}
    return NULL;
}

extern char *
NET_netbuf_index (NET_netbuf_t * buf)
{
	return buf->index;
}

extern ACE_bool_t
NET_netbuf_index_inc (NET_netbuf_t * buf, long inc)
{
	ACE_bool_t ok = FALSE;
	if ( (buf->index + inc) >= buf->data &&
	     (buf->index + inc) <= (buf->data + buf->size) ){
    	buf->index += inc;
    	buf->len -= inc;
    	ok = TRUE;
	}
	return ok;
}

extern ACE_bool_t
NET_netbuf_trim_len (NET_netbuf_t * buf, unsigned long len)
{
	ACE_bool_t ok = FALSE;
    if (len <= buf->len)
    {
        buf->len = len;
        ok = TRUE;
    }
    return ok;
}

extern long
NET_netbuf_len (NET_netbuf_t * buf)
{
    long len = 0;
	if (buf != NULL){
		len = buf->len;
	}
    return len;
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
