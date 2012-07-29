/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/stdio.h>

static USO_heap_t *global_heap = NULL;
static void (*abort) (char *msg, char *file, int line) = NULL;
static void (*panic) (char *msg, char *file, int line) = NULL;

extern void
ACE_stdlib_init (USO_heap_t * heap,
                 void (*abort_handler) (char *msg, char *file, int line),
                 void (*panic_handler) (char *msg, char *file, int line))
{
    global_heap = heap;
    abort = abort_handler;
    panic = panic_handler;
}

extern void *
ACE_malloc (ACE_size_t size)
{
    return USO_mem_alloc (global_heap, size);
}

extern void
ACE_free (void *block)
{
    USO_mem_free (global_heap, block);
}

extern void
ACE_abort (char *msg, char *file, int line)
{
    if (abort != NULL)
    {
        abort (msg, file, line);
    }
    for (;;);
}

extern void
ACE_panic (char *msg, char *file, int line)
{
    if (panic != NULL)
    {
        panic (msg, file, line);
    }
    for (;;);
}
