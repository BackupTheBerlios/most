/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_DEBUG_H
#define DEV_DEBUG_H

#include "uso/debug.h"

/** @addtogroup dev
 * @{
 */

/** @addtogroup debug
 *
 * @{
 */

#if DEV_DEBUGGING

#define DEV_FLASH_DEBUG      1
#define DEV_AT91_DEBUG       1
#define DEV_SMC_DEBUG        1
#define DEV_MAC_DEBUG        1

#else

#define DEV_FLASH_DEBUG      0
#define DEV_AT91_DEBUG       0
#define DEV_SMC_DEBUG        0
#define DEV_MAC_DEBUG        1

#endif

/** @}
 */

/** @}
 */

#endif
