#ifndef SAM_EXCEPTIONS_H
#define SAM_EXCEPTIONS_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup exceptions exceptions.h
 *
 * Exceptions.
 *
 * @{
 */

/**
 *
 */
extern void SAM_undefined_instruction (void);

/**
 *
 */
extern void SAM_prefetch_abort (void);

/**
 *
 */
extern void SAM_data_abort (void);


/**
 *
 */
extern void SAM_default_spurious_handler (void);

/**
 *
 */
extern void SAM_default_irq_handler (void);

/**
 *
 */
extern void SAM_default_fiq_handler (void);


/**
 *
 */
extern void SAM_sys_interrupt_init (void);

/**
 *
 */
extern void SAM_emac_interrupt_init (void);

/** @} */

/** @} */

#endif
