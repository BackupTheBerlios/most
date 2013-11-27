/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/stack.h>

extern void
USO_stack_init (USO_stack_t stack[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        stack[i] = USO_STACK_INIT_VALUE;
    }
}

extern ACE_bool_t
USO_stack_check_free (USO_stack_t * end)
{
    return *end == USO_STACK_INIT_VALUE ? TRUE : FALSE;
}

#if defined USO_STACK_HI2LOW

extern USO_stack_t *
USO_stack_beginn (USO_stack_t stack[], int size)
{
    return &stack[size - 1];
}

extern USO_stack_t *
USO_stack_end (USO_stack_t stack[], int size)
{
    return &stack[0];
}

extern ACE_bool_t
USO_stack_check_overrun (USO_stack_t * end, USO_cpu_register_t sp)
{
    return (USO_stack_t *) sp < end ? TRUE : FALSE;
}

extern int
USO_stack_get_free (USO_stack_t * end, int size)
{
    int free;
    for (free = 0; free <= size; ++free)
    {
        if (*end != USO_STACK_INIT_VALUE)
            break;
        ++end;
    }
    return free;
}

#elif defined USO_STACK_LOW2HI

extern USO_stack_t *
USO_stack_beginn (USO_stack_t stack[], int size)
{
    return &stack[0];
}

extern USO_stack_t *
USO_stack_end (USO_stack_t stack[], int size)
{
    return &stack[size - 1];
}

extern bool_t
USO_stack_check_overrun (USO_stack_t * end, USO_cpu_register_t sp)
{
    return (USO_stack_t *) sp > end ? TRUE : FALSE;
}

extern int
USO_stack_get_free (USO_stack_t * end, int size)
{
    int free;
    for (free = 0; free <= size; ++free)
    {
        if (*end != USO_STACK_INIT_VALUE)
            break;
        --end;
    }
    return free;
}

#else
#error "Kind of stack not defined!"
#endif
