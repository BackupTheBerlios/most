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
    NET_BUF_POOL, /**< POOL */
    NET_BUF_TRANS, /**< Transport */
	NET_BUF_FREE /** < netbuf is already free */
};

/** Networ buffer */
typedef struct NET_netbuf NET_netbuf_t;

extern void NET_netbuf_init (void);

/**
 * ?
 */
extern NET_netbuf_t *NET_netbuf_alloc_trans (void);

extern NET_netbuf_t *NET_netbuf_alloc_pool (USO_buf_pool_t * pool);

extern NET_netbuf_t *NET_netbuf_alloc_ram (long size);

extern NET_netbuf_t *NET_netbuf_alloc_rom ( char *data, long size);



/**
 * ?
 */
extern void NET_netbuf_free (NET_netbuf_t *);

/**
 * ?
 */
extern NET_netbuf_t* NET_netbuf_chain (NET_netbuf_t *, NET_netbuf_t *);

/**
 * ?
 */
extern NET_netbuf_t* NET_netbuf_next (NET_netbuf_t * buf);

/**
 * ?
 */
extern char * NET_netbuf_index (NET_netbuf_t * buf);

/**
 * ?
 */
extern bool_t NET_netbuf_index_inc (NET_netbuf_t *, long);

/**
 * ?
 */
extern bool_t NET_netbuf_trim_len (NET_netbuf_t *, unsigned long);

/**
 * ?
 */
extern long NET_netbuf_len (NET_netbuf_t * buf);

/**
 * ?
 */
extern long NET_netbuf_tot_len (NET_netbuf_t *);

/** @}
 */

#endif
