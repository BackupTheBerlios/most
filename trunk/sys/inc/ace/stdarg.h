/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDARG_H
#define ACE_STDARG_H

/** @addtogroup ace
 *
 * @{
 */

/** @defgroup stdarg stdarg.h
 *
 * Standard argument definitions.
 *
 * @{
 */

/*------------- Definitions ------------------------------------------*/

/** Variable argument list type. */
typedef void *ACE_va_list_t;

/** Variable argument symbol. */
#define ACE_va_ellipsis ...

#define ACE_va_rounded_size(TYPE) (((sizeof (TYPE) + sizeof (long) - 1) / sizeof (long)) * sizeof (long))

/** Set argument pointer to beginning of list of optional arguments.*/
#define ACE_va_start(AP,LASTARG) (AP = ((ACE_va_list_t) __builtin_next_arg (LASTARG)))

/** Retrieve current argument.*/
#define ACE_va_arg(AP, TYPE) \
 (AP = (ACE_va_list_t) ((char *) (AP) + ACE_va_rounded_size (TYPE)),	\
  *((TYPE *) (void *) ((char *) (AP)					\
		       - ((sizeof (TYPE) < ACE_va_rounded_size (int)	\
			  ? sizeof (TYPE) : ACE_va_rounded_size (TYPE))))))

/** Reset argument pointer.*/
#define ACE_va_end(AP)	((void) 0)

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
