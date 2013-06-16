/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_DEBUG_H
#define LA2_DEBUG_H

#include "uso/debug.h"

/** @addtogroup init
 *
 * @{
 */

/** @defgroup debug debug.h
 *
 * If NET_DEBUGGING is defined.
 *
 * @{
 */

#if BSP_DEBUGGING

#define LA2_INIT_DEBUG      1

#else

#define LA2_INIT_DEBUG      0

#endif

/** @}
 */

/** @}
 */

#endif
