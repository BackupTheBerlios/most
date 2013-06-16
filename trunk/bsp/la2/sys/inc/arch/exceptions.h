#ifndef LA2_EXCEPTIONS_H
#define LA2_EXCEPTIONS_H

#include <ace/stddef.h>

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
extern void LA2_default_reset_handler (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_undefined_instruction (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_software_interrupt (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_prefetch_abort (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_data_abort (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_default_irq_handler (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_default_fiq_handler (void) ACE_SECTION_ (".unref");

/**
 *
 */
extern void LA2_default_spurious_handler (void) ACE_SECTION_ (".unref");

/** @} */

/** @} */

#endif
