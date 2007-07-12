/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_HEAP_H
#define USO_HEAP_H

#include "ace/stddef.h"

/** @defgroup heap heap.h
 *
 * Memory management.
 * @{
 */

/*-------------- Representation ------------------------------------------*/

typedef struct USO_mblock USO_mblock_t;

typedef struct USO_fblock USO_fblock_t;

struct USO_heap
{
	USO_mblock_t *mem_blocks;
	USO_fblock_t *free_blocks;
	unsigned long free_mem;
	unsigned long total_mem;
	unsigned long alloc_count;
	unsigned long search_count;
	unsigned long search_average;
};

typedef struct USO_heap USO_heap_t;

/*-------------- Interface -----------------------------------------------*/


/**
 * Initialize heap.
 *
 * @param heap : Pointer to heap.
 * @param start : Pointer to start of the heap.
 * @param end : Pointer to the end of the heap.
 */
extern bool_t USO_heap_init (USO_heap_t* heap, void *start, void *end);

extern void USO_heap_install(USO_heap_t* heap, char *name);

/**
 * Initialize heap.
 *
 * @param heap : Heap.
 * @param size : Size of the memory you want.
 *
 * @return Pointer to memory block or NULL if no memory avilable.
 */
extern void *USO_mem_alloc (USO_heap_t* heap, size_t size);

/** 
 * Fremm memory block.
 *
 * @param heap : Heap.
 * @param block : valid memory block.
 */
extern void USO_mem_free (USO_heap_t* heap, void *block);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
