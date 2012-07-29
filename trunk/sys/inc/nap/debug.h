/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef NAP_DEBUG_H
#define NAP_DEBUG_H

#include "uso/debug.h"

/** @addtogroup nap
 * @{
 */

/** @addtogroup debug
 *
 * If NAP_DEBUGGING is defined.
 *
 * @{
 */

#if NAP_DEBUGGING

#define NAP_BOOTP_DEBUG         1 /**< NAP_DEBUGGING on */
#define NAP_TFTP_DEBUG          1 /**< NAP_DEBUGGING on */
#define NAP_SYSLOG_DEBUG        1 /**< NAP_DEBUGGING on */

#else

#define NAP_BOOTP_DEBUG         0 /**< NAP_DEBUGGING off */
#define NAP_TFTP_DEBUG          0 /**< NAP_DEBUGGING off */
#define NAP_SYSLOG_DEBUG        0 /**< NAP_DEBUGGING off */

#endif

/** @}
 */

/** @}
 */

#endif
