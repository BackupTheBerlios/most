/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_STACK_H
#define USO_STACK_H

#include "uso/arch/cpu.h"

/** @defgroup stack stack.h
 *
 * On different cpu architectures, the stack grows 
 * from bottom to top or top to bottom.
 * For that reason use this functions. 
 *
 * @{
 */

/*-------------- Interface -----------------------------------------------*/

/**
 * Returns the beginn of a stack.
 *
 * @param stack : The stack, an array of type USO_stack_t.
 * @param size : How many entities of stack_t in the array.
 * @return Beginn of the stack.
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

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
