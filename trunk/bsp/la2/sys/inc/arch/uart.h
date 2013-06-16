/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_SCI_H
#define LA2_SCI_H

#include <ace/stddef.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup uart uart.h
 *
 * Serial Communication Interfaces
 *
 * @{
 */

/**
 * Initialize serial com. interface 0.
 */
void LA2_uart_init_0 (void);

/**
 * Initialize serial com. interface 1.
 */
void LA2_uart_init_1 (void);

/**
 * Uart 0 interrupt.
 */
extern void LA2_uart_interrupt_0 (void) ACE_SECTION_ (".unref");

/**
 * Uart 1 interrupt.
 */
extern void LA2_uart_interrupt_1 (void) ACE_SECTION_ (".unref");


/** @}
 */

/** @}
 */


#endif
