/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_LIST_H
#define USO_LIST_H

#include <ace/stddef.h>

/** @defgroup list list.h
 *
 * Queue, Stack, Delta List 
 * @{
 */

/*------------- Representation ------------------------------------------*/

/**
 * Node type.
 */
typedef struct USO_node USO_node_t;

/*
 * Node struct.
 *
 * Private.
 */
struct USO_node
{
    USO_node_t *next;
    USO_node_t *prev;
    long data;
};

/*
 * List struct.
 *
 * Private.
 */
struct USO_list
{
    USO_node_t *head;
    USO_node_t *tail;
};

/**
 * List type.
 */
typedef struct USO_list USO_list_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize list.
 *
 * @param list : Pointer to list.
 */
extern void USO_list_init (USO_list_t * list);

/**
 * Remove node from list.
 *
 * @param list : Must contain the node.
 * @param node : Will be removed from list.
 */
extern void USO_remove (USO_list_t * list, USO_node_t * node);

/**
 * Enqueue node to the list (tail).
 *
 * @param list : Pointer to list.
 * @param node : Pointer to node.
 */
extern void USO_enqueue (USO_list_t * list, USO_node_t * node);

/**
 * Push node to the list (head).
 *
 * @param list : Pointer to list.
 * @param node : Pointer to node.
 */
extern void USO_push (USO_list_t * list, USO_node_t * node);

/**
 * Dequeue node from the list (head).
 *
 * @param list : Pointer to list.
 * @return Pointer to node or NULL if list is empty.
 */
extern USO_node_t *USO_dequeue (USO_list_t * list);

/**
 * Pop node from the list (head).
 *
 * @param list : Pointer to list.
 * @return Pointer to node or NULL if list is empty.
 */
extern USO_node_t *USO_pop (USO_list_t * list);

/**
 * Insert node into list using an delta list algorithm.
 *
 * @param list : Pointer to list.
 * @param node : Pointer to node.
 * @param key : Value for calculating delta.
 */
extern void USO_delta_insert (USO_list_t * list, USO_node_t * node, long key);

/**
 * Remove a node from a delta list.
 *
 * @param list : Pointer to delta list.
 * @param node : Pointer to node.
 */
extern void USO_delta_remove (USO_list_t * list, USO_node_t * node);

/**
 * Decrement delta value of the head of the delta list.
 *
 * @param list : Pointer to delta list.
 */
extern void USO_delta_dec (USO_list_t * list);

/**
 * Are there nodes in the list whit delta 0.
 *
 * @param list : Pointer to delta list.
 * @return TRUE when delta of head <= 0 else FALSE.
 */
extern ACE_bool_t USO_delta_null (USO_list_t * list);

/**
 * Is the list empty.
 *
 * @param list : Pointer to list.
 * @return TRUE if empty else FALSE..
 */
extern ACE_bool_t USO_isempty (USO_list_t * list);

/**
 * Get the next element in the list.
 * The element is not removed from the list.
 *
 * @param list : Pointer to list.
 * @param iterator : Iterator(Pointer to current node or NULL for list head).
 * @return Element.
 */
extern USO_node_t *USO_next_element (USO_list_t * list, USO_node_t *iterator);

/**
 * Get the previous element in the list.
 * The element is not removed from the list.
 *
 * @param list : Pointer to list.
 * @param iterator : Iterator.
 * @return Element.
 */
extern USO_node_t *USO_prev_element (USO_list_t * list, USO_node_t *iterator);

/**
 * Do function f on each element of the list.
 *
 * Beginning with the head as long f returns FALSE,
 * use f on the next node, when f returns TRUE, the node
 * on which f was used will be returned.
 *
 * @param list : Pointer to list.
 * @param f : Function used on each node.
 * @return Node if one was found or NULL.
 */
extern void USO_map (USO_list_t *list, void (*f) (USO_node_t *) );

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
