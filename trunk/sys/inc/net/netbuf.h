/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_NETBUF_H
#define NET_NETBUF_H

#include <uso/list.h>
#include <uso/buf_pool.h>

/** @addtogroup net
 * @{
 */

/** @defgroup netbuf netbuf.h
 *
 * Network packet buffer.
 *
 * @{
 */

/** Network packet buffer types. */
enum NET_buf_type
{
    NET_BUF_ROM,    /**< The buffer data is just a pointer to any data. */
    NET_BUF_RAM,    /**< Buffer is allocated from RAM. */
    NET_BUF_POOL,   /**< Buffer is allocated from memory pool. */
    NET_BUF_TRANS,  /**< Transport buffer. */
    NET_BUF_FREE        /**< Netbuf is already released (to avoid doing a mem_free on a buffer which is already free. */
};

/** Network buffer type. */
typedef struct NET_netbuf NET_netbuf_t;

/**
 * Initialize network buffer pools.
 */
extern void NET_netbuf_init (void);

/**
 * Get a transfer buffer.
 * @return Network buffer.
 */
extern NET_netbuf_t *NET_netbuf_alloc_trans (void);

/**
 * Get a buffer from a buffer pool.
 * @return Network buffer.
 */
extern NET_netbuf_t *NET_netbuf_alloc_pool (USO_buf_pool_t * pool);

/**
 * Get a buffer from heap.
 * @param size : Size of the data(buffer).
 * @return Network buffer.
 */
extern NET_netbuf_t *NET_netbuf_alloc_ram (long size);

/**
 * Get a buffer where data points to data outside the buffer.
 * @param data : Pointer to data.
 * @param size : Size of the data.
 * @return Network buffer.
 */
extern NET_netbuf_t *NET_netbuf_alloc_rom (char *data, long size);



/**
 * Release network buffer.
 * @param buf : Network buffer.
 */
extern void NET_netbuf_free (NET_netbuf_t * buf);

/**
 * Chain buffer b two the end of the buffer chain from buffer a.
 * @param a : Network buffer.
 * @param b : Network buffer.
 * @return Chained network buffer.
 */
extern NET_netbuf_t *NET_netbuf_chain (NET_netbuf_t * a, NET_netbuf_t * b);

/**
 * Get the next buffer in the buffer chain.
 * @param buf : Network buffer.
 * @return Next buffer or NULL.
 */
extern NET_netbuf_t *NET_netbuf_next (NET_netbuf_t * buf);

/**
 * Get the current index pointing to data in the buffer.
 * @param buf : Network buffer.
 * @return Index.
 */
extern char *NET_netbuf_index (NET_netbuf_t * buf);

/**
 * Adjust the index of the buffer, the increment can be positiv or negativ.
 * The index is checked for buffer boundaries.
 * @param buf : Network buffer.
 * @param inc : Positive or negative increment.
 * @return TRUE is the index is adjusted, else FALSE.
 */
extern ACE_bool_t NET_netbuf_index_inc (NET_netbuf_t * buf, long inc);

/**
 * Trim the buffer length to given length.
 * You can not increase the buffer length.
 * @param buf : Network buffer.
 * @param len : Length to which the buffer should be trimmed.
 * @return TRUE if buffer length was trimmed else FALSE.
 */
extern ACE_bool_t NET_netbuf_trim_len (NET_netbuf_t * buf, unsigned long len);

/**
 * Get the buffer length (This is not the total length of all chained buffers).
 * @param buf : Network buffer.
 * @return Length.
 */
extern long NET_netbuf_len (NET_netbuf_t * buf);

/**
 * Calculate total length over the buffer chain.
 * @param buf : Network buffer.
 * @return Total length.
 */
extern long NET_netbuf_tot_len (NET_netbuf_t * buf);

/** @}
 */

/** @}
 */


#endif
