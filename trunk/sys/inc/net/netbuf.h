/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_NETBUF_H
#define NET_NETBUF_H

#include <uso/list.h>
#include <uso/buf_pool.h>

/** @defgroup netbuf netbuf.h
 *
 * Network buffer.
 *
 * @{
 */

/** ? */
enum NET_buf_type
{
    NET_BUF_ROM, /**< ROM */
    NET_BUF_RAM, /**< RAM */
    NET_BUF_POOL /**< POOL */
};

/** Networ buffer */
typedef struct NET_netbuf NET_netbuf_t;

/** private */
struct NET_netbuf
{
    USO_node_t node;
    NET_netbuf_t *next;
    char *data;
    long size;
    char *index;
    long len;
    USO_buf_pool_t *pool;
    enum NET_buf_type type;
};


/**
 * ?
 */
extern NET_netbuf_t *NET_netbuf_alloc (USO_buf_pool_t *, long, char *);

/**
 * ?
 */
extern void NET_netbuf_free (NET_netbuf_t *);

/**
 * ?
 */
extern NET_netbuf_t *NET_netbuf_chain (NET_netbuf_t *, NET_netbuf_t *);

/**
 * ?
 */
extern void NET_netbuf_index_inc (NET_netbuf_t *, long);

/**
 * ?
 */
extern bool_t NET_netbuf_len_adjust (NET_netbuf_t *, unsigned long);

/**
 * ?
 */
extern long NET_netbuf_tot_len (NET_netbuf_t *);

/** @}
 */

#endif
