/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_SCI_H
#define SAM_SCI_H

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
void SAM_uart_init_0 (void);

/**
 * Initialize serial com. interface 1.
 */
void SAM_uart_init_1 (void);

/**
 * Uart 0 interrupt.
 */
extern void SAM_uart_interrupt_0 (void);

/**
 * Uart 1 interrupt.
 */
extern void SAM_uart_interrupt_1 (void);


/** @}
 */

/** @}
 */


#endif
