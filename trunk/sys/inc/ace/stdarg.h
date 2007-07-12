/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef ACE_STDARG_H
#define ACE_STDARG_H

/** @defgroup stdarg stdarg.h
 *
 * Standard argument definitions.
 *
 * @{
 */

/*------------- Definitions ------------------------------------------*/

/** variable list type */
typedef void *va_list_t;

/** variable argument symbol */
#define va_ellipsis ...

/** va_rounded_size */
#define va_rounded_size(TYPE) (((sizeof (TYPE) + sizeof (long) - 1) / sizeof (long)) * sizeof (long))

/** va_start */
#define va_start(AP,LASTARG) (AP = ((va_list_t) __builtin_next_arg (LASTARG)))

/** va_arg */
#define va_arg(AP, TYPE) \
 (AP = (va_list_t) ((char *) (AP) + va_rounded_size (TYPE)),	\
  *((TYPE *) (void *) ((char *) (AP)					\
		       - ((sizeof (TYPE) < va_rounded_size (int)	\
			  ? sizeof (TYPE) : va_rounded_size (TYPE))))))

/** va_end */
#define va_end(AP)	((void) 0)

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
