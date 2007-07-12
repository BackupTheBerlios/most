/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>

static USO_heap_t* global_heap = NULL;

extern void
ACE_stdlib_init (USO_heap_t* heap)
{
	global_heap = heap;
}

extern void *malloc (size_t size)
{
	return USO_mem_alloc (global_heap, size);
}

extern void free (void *block)
{
	USO_mem_free (global_heap, block);
}
