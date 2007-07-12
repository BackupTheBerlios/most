/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/list.h"
#include "uso/heap.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_list_init (USO_list_t * list)
{
    list->head = NULL;
    list->tail = NULL;
}

static void
put_first (USO_list_t * list, USO_node_t * node)
{
    node->next = NULL;
    node->prev = NULL;
    list->head = node;
    list->tail = node;
}

static void
put_head (USO_list_t * list, USO_node_t * node)
{
    node->next = list->head;
    node->prev = NULL;
    list->head->prev = node;
    list->head = node;
}

static void
put_tail (USO_list_t * list, USO_node_t * node)
{
    node->next = NULL;
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
}

static USO_node_t *
get_head (USO_list_t * list)
{
    USO_node_t *node;
    node = list->head;
    if (node->next != NULL)
    {
        node->next->prev = NULL;
    }
    else
    {
        list->tail = NULL;
    }
    list->head = node->next;
    return node;
}

static USO_node_t *
get_tail (USO_list_t * list)
{
    USO_node_t *node;
    node = list->tail;
    if (node->prev != NULL)
    {
        node->prev->next = NULL;
    }
    else
    {
        list->head = NULL;
    }
    list->tail = node->prev;
    return node;
}

static void
insert (USO_node_t * next_node, USO_node_t * node)
{
    node->next = next_node;
    node->prev = next_node->prev;
    next_node->prev->next = node;
    next_node->prev = node;
}

static void
remove (USO_node_t * node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

extern void
USO_remove (USO_list_t * list, USO_node_t * node)
{
    if (list->head == node)
    {
        get_head (list);
    }
    else if (list->tail == node)
    {
        get_tail (list);
    }
    else
    {
        remove (node);
    }
}

extern void
USO_enqueue (USO_list_t * list, USO_node_t * node)
{
    if (USO_isempty (list) == TRUE)
    {
        put_first (list, node);
    }
    else
    {
        put_tail (list, node);
    }
}

extern USO_node_t *
USO_dequeue (USO_list_t * list)
{
    USO_node_t *node;
    if (USO_isempty (list) == TRUE)
    {
        node = NULL;
    }
    else
    {
        node = get_head (list);
    }
    return node;
}

extern void
USO_push (USO_list_t * list, USO_node_t * node)
{
    if (USO_isempty (list) == TRUE)
    {
        put_first (list, node);
    }
    else
    {
        put_head (list, node);
    }
}

extern USO_node_t *
USO_pop (USO_list_t * list)
{
    USO_node_t *node;
    if (USO_isempty (list) == TRUE)
    {
        node = NULL;
    }
    else
    {
        node = get_head (list);
    }
    return node;
}

extern void
USO_delta_insert (USO_list_t * list, USO_node_t * node, long key)
{
    if (USO_isempty (list) == TRUE)
    {
        node->data = key;
        put_first (list, node);
    }
    else
    {
        USO_node_t *next_node;
        for (next_node = list->head; next_node != NULL;
             next_node = next_node->next)
        {
            if (key <= next_node->data)
            {
                next_node->data -= key;
                node->data = key;
                if (next_node == list->head)
                {
                    put_head (list, node);
                }
                else
                {
                    insert (next_node, node);
                }
                break;
            }
            else
            {
                key -= next_node->data;
            }
        }
        if (next_node == NULL)
        {
            node->data = key;
            put_tail (list, node);
        }
    }
}

extern void
USO_delta_remove (USO_list_t * list, USO_node_t * node)
{
    if (node->next != NULL)
    {
        node->next->data += node->data;
    }
    USO_remove (list, node);
}

extern void
USO_delta_dec (USO_list_t * list)
{
    if (USO_isempty (list) == FALSE)
    {
        --list->head->data;
    }
}

extern bool_t
USO_delta_null (USO_list_t * list)
{
    if (USO_isempty (list) == FALSE)
    {
        if (list->head->data <= 0)
            return TRUE;
    }
    return FALSE;
}

extern bool_t
USO_isempty (USO_list_t * list)
{
    if (list->head == NULL)
        return TRUE;
    return FALSE;
}

extern USO_node_t *
USO_next_element (USO_list_t * list, USO_node_t *iterator)
{
    if (iterator == NULL){
    	iterator=list->head;
    }else{
        iterator = iterator->next;
    }
    return iterator;
}

extern USO_node_t *
USO_prev_element (USO_list_t * list, USO_node_t *iterator)
{
    if (iterator == NULL){
    	iterator = list->tail;
    } else {
        iterator = iterator->prev;
    }
    return iterator;
}

extern void
USO_map (USO_list_t * list, void (*f) (USO_node_t *) )
{
    USO_node_t *node;
    for (node = list->head; node != NULL; node = node->next)
    {
        f(node); 
    }
}

/*------------------------------------------------------------------------*/
