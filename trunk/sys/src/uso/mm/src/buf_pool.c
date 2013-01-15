/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

#include "uso/buf_pool.h"
#include "uso/cpu.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_buf_pool_init (USO_buf_pool_t * buf_pool, void *buf, int count, long size)
{
    int i;
    buf_pool->buf = buf;
    buf_pool->buf_size = size;
    buf_pool->buf_count = count;
    USO_list_init (&buf_pool->free_buf);
    for (i = 0; i < count; ++i)
    {
        USO_enqueue (&buf_pool->free_buf, (USO_node_t *) buf);
        buf = (char *)buf + size;
    }
    USO_semaphore_init (&buf_pool->sem, count);
}

extern USO_buf_pool_t *
USO_buf_pool_new (int count, long size)
{
    USO_buf_pool_t *buf_pool = ACE_malloc (sizeof (USO_buf_pool_t));
    if (buf_pool)
    {
        void *buf = ACE_malloc (count * size);
        if (buf)
        {
            USO_buf_pool_init (buf_pool, buf, count, size);
        }
        else
        {
            ACE_free (buf_pool);
            buf_pool = NULL;
        }
    }
    return buf_pool;
}

extern void
USO_buf_pool_del (USO_buf_pool_t * buf_pool)
{
    ACE_free (buf_pool->buf);
    ACE_free (buf_pool);
}

extern void *
USO_buf_alloc (USO_buf_pool_t * buf_pool)
{
    USO_node_t *buf;
    USO_cpu_status_t ps = USO_disable ();
    USO_wait (&buf_pool->sem);
    buf = USO_dequeue (&buf_pool->free_buf);
    USO_restore (ps);
    return buf;
}

extern void
USO_buf_free (USO_buf_pool_t * buf_pool, void *buf)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_enqueue (&buf_pool->free_buf, (USO_node_t *) buf);
    USO_signal (&buf_pool->sem);
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
