/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/stack.h"
#include "uso/arch/cpu.h"

#ifdef USO_STACK
#if USO_STACK==USO_HI2LOW

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

extern bool_t 
USO_stack_check_overrun(USO_stack_t *end, USO_cpu_register_t act)
{
	return (USO_stack_t*)act < end ? TRUE : FALSE; 
}

#elif USO_STACK==USO_LOW2HI

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
USO_stack_check(USO_stack_t *end, USO_cpu_register_t act)
{
	return (USO_stack_t*)act > end ? TRUE : FALSE; 
}

#endif
#endif
