/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_DEBUG_H
#define CLI_DEBUG_H

#include <uso/debug.h>

/** @addtogroup net
 * @{
 */

/** @addtogroup debug
 *
 * If CLI_DEBUGGING is defined.
 *
 * @{
 */

#if CLI_DEBUGGING

#define CLI_INT_DEBUG      1   /**< CLI interpreter debug on */

#else

#define CLI_INT_DEBUG      0 /**< CLI interpreter debug off */

#endif

/** @}
 */

/** @}
 */

#endif
