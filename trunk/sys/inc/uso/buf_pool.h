/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_BUF_POOL_H
#define USO_BUF_POOL_H

#include "uso/list.h"
#include "uso/semaphore.h"


/** @defgroup buf_pool buf_pool.h
 *
 * Blocking memory management.
 * When you init a buf_pool or free a buffer, a USO_node_t
 * is written to the beginning of the buffer.
 * When you get a buffer it is not zero initialized!
 * 
 * The size of a buffer has to be aligned to 4 Bytes,
 * or a address boundary where a pointer can be placed.
 *
 * Buffer pools should be used for message passing and IO buffering.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Buffer pool struct.
 *
 * Private.
 */
struct USO_buf_pool
{
    USO_list_t free_buf;
    USO_semaphore_t sem;
    void *buf;
    long buf_size;
    int buf_count;
};

/**
 * Buffer pool type.
 *
 */
typedef struct USO_buf_pool USO_buf_pool_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a buffer pool.
 *
 * @param buf_pool : Pointer to buffer pool.
 * @param buf : Pointer to buffer array.
 * @param count : Amount of the buffers in the array.
 * @param size : Size of one buffer.
 */
extern void USO_buf_pool_init (USO_buf_pool_t * buf_pool,
                               void *buf, int count, long size);

/**
 * Create a buffer pool.
 *
 * @param count : Amount of the buffers in the array.
 * @param size : Size of one buffer.
 */
extern USO_buf_pool_t *USO_buf_pool_new (int count, long size);

/**
 * Delete a buffer pool.
 * 
 * @param buf_pool : Pointer to pool.
 */
extern void USO_buf_pool_del (USO_buf_pool_t * buf_pool);

/**
 * Allocate a buffer.
 * The function blocks if no buffer is free.
 *
 * @param buf_pool : Pointer to buffer pool.
 * @return Buffer.
 */
extern void *USO_buf_alloc (USO_buf_pool_t * buf_pool);

/** 
 * Give a buffer back to the free buffer list.
 *
 * @param buf_pool : Pointer to buffer pool.
 * @param buf : Buffer which should be given back to free list.
 */
extern void USO_buf_free (USO_buf_pool_t * buf_pool, void *buf);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
