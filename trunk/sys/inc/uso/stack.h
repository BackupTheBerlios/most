/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_STACK_H
#define USO_STACK_H

#include <uso/cpu.h>

/** @addtogroup uso
 * @{
 */

/** @defgroup stack stack.h
 *
 * On different CPU architectures, the stack grows
 * from bottom to top or top to bottom.
 * For that reason use this functions. 
 *
 * @{
 */

#define USO_STACK_INIT_VALUE ((USO_stack_t)0xA5A5A5A5) /**< Stack will be initialized with this value. */

/*-------------- Interface -----------------------------------------------*/

/**
 * @param stack : The stack, an array of type USO_stack_t.
 * @param size : How many entities of stack_t in the array.
 */
extern void USO_stack_init (USO_stack_t stack[], int size);

/**
 * Check if the entry on the end of the stack is still free, if not an stack overflow has occured.
 *
 * @param end : Pointer to the last entry of the stack.
 * @return TRUE if last entry is free, else FALSE(stack overflow).
 */
extern ACE_bool_t USO_stack_check_free (USO_stack_t * end);

/**
 * Returns the beginn of a stack.
 *
 * @param stack : The stack, an array of type USO_stack_t.
 * @param size : How many entities of stack_t in the array.
 * @return Begin of the stack.
 */
extern USO_stack_t *USO_stack_beginn (USO_stack_t stack[], int size);

/**
 * Returns the end of a stack.
 *
 * @param stack : The stack, an array of type USO_stack_t.
 * @param size : How many entities of stack_t in the array.
 * @return End of the stack.
 */
extern USO_stack_t *USO_stack_end (USO_stack_t stack[], int size);

/**
 * Check if the current stack pointer has run out of the stack end.
 * This function may be called from the scheduler.
 *
 * @param end : The end (top, bottom) of the stack.
 * @param sp : Current stack pointer.
 * @return TRUE if a stack overrun has occur.
 */
extern ACE_bool_t USO_stack_check_overrun (USO_stack_t * end, USO_cpu_register_t sp);

/**
 * Determine the free space of a stack.
 *
 * @param end : Pointer to the last entry of the stack.
 * @param size : Size of the stack.
 * @return Free entries on the stack.
 */
extern int USO_stack_get_free (USO_stack_t * end, int size);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
