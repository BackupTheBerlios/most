/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/heap.h>

#include "dev/digin.h"

USO_list_t inputs;

extern void
DEV_digin_init (DEV_digin_t * in,
                enum DEV_digio_logig logig,
                unsigned long (*sample) (void), int debounce_time)
{
    in->logig = logig;
    in->sample = sample;
    if (debounce_time < 1)
        debounce_time = 1;
    in->debounce_time = debounce_time;
    in->debounce_count = 0;
    if (sample ())
        in->state = DEV_DIGIO_HIGH;
    in->pos_edge_count = 0;
    in->neg_edge_count = 0;
    USO_semaphore_init (&in->pos_edge_sem, 0);
    USO_semaphore_init (&in->neg_edge_sem, 0);
    USO_enqueue (&inputs, (USO_node_t *) in);
}

static void
pos_edge (DEV_digin_t * in)
{
    while (in->pos_edge_count)
    {
        --in->pos_edge_count;
        USO_signal (&in->pos_edge_sem);
    }
}

static void
neg_edge (DEV_digin_t * in)
{
    while (in->neg_edge_count)
    {
        --in->neg_edge_count;
        USO_signal (&in->neg_edge_sem);
    }
}

static void
sample (USO_node_t * in)
{
    if (((DEV_digin_t *) in)->debounce_count == 0)
    {
        unsigned long s = ((DEV_digin_t *) in)->sample ();
        if ((s && (((DEV_digin_t *) in)->state == DEV_DIGIO_LOW)) ||
            (!s && (((DEV_digin_t *) in)->state == DEV_DIGIO_HIGH)))
        {
            ((DEV_digin_t *) in)->debounce_count =
                ((DEV_digin_t *) in)->debounce_time;
        }
    }
    else
    {
        --((DEV_digin_t *) in)->debounce_count;
        if (((DEV_digin_t *) in)->debounce_count == 0)
        {
            unsigned long s = ((DEV_digin_t *) in)->sample ();
            if (s && (((DEV_digin_t *) in)->state == DEV_DIGIO_LOW))
            {
                ((DEV_digin_t *) in)->state = DEV_DIGIO_HIGH;
                if (((DEV_digin_t *) in)->logig == DEV_DIGIO_POS)
                    pos_edge ((DEV_digin_t *) in);
                else
                    neg_edge ((DEV_digin_t *) in);
            }
            else if (!s && (((DEV_digin_t *) in)->state == DEV_DIGIO_HIGH))
            {
                ((DEV_digin_t *) in)->state = DEV_DIGIO_LOW;
                if (((DEV_digin_t *) in)->logig == DEV_DIGIO_POS)
                    neg_edge ((DEV_digin_t *) in);
                else
                    pos_edge ((DEV_digin_t *) in);
            }
        }
    }
}

extern void
DEV_digin_sample (void)
{
    USO_map (&inputs, sample);
}

extern bool_t
DEV_digin_isset (DEV_digin_t * in)
{
    if (in->logig == DEV_DIGIO_POS)
    {
        if (in->state == DEV_DIGIO_HIGH)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if (in->state == DEV_DIGIO_HIGH)
            return FALSE;
        else
            return TRUE;
    }
}

extern void
DEV_digin_wait (DEV_digin_t * in, enum DEV_digio_state state, bool_t level)
{
    if (level == TRUE)
    {
        if (in->logig == DEV_DIGIO_POS)
        {
            if (state == in->state)
                return;
        }
        else
        {
            if (state != in->state)
                return;
        }
    }
    if (state == DEV_DIGIO_HIGH)
    {
        in->pos_edge_count++;
        USO_wait (&in->pos_edge_sem);
    }
    else
    {
        in->neg_edge_count++;
        USO_wait (&in->neg_edge_sem);
    }
}
