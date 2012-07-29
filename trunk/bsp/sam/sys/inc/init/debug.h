/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_DEBUG_H
#define SAM_DEBUG_H

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

#define SAM_INIT_DEBUG      1

#else

#define SAM_INIT_DEBUG      0

#endif

/** @}
 */

/** @}
 */

#endif
