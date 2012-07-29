/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_DEBUG_H
#define USO_DEBUG_H

#include "uso/log.h"

/** @addtogroup uso
 * @{
 */

/** @defgroup debug debug.h
 *
 * Debug definitions for developing.
 *
 * Turn on debugging at compile time.
 * @{
 */

/*------------- Definitions ------------------------------------------*/

#define DEBUGGING 1

#if DEBUGGING

/**
 * Debug function switched on and of at compiletime.
 */
#define DEBUGF(debug, x) do { if(debug){ USO_debug x; } } while(0)

#define USO_DEBUGGING        1     /**< Enable debugging for USO library. */
#define DEV_DEBUGGING        1     /**< Enable debugging for DEV library. */
#define NET_DEBUGGING        0     /**< Enable debugging for NET library. */
#define NAP_DEBUGGING        0     /**< Enable debugging for NAP library. */
#define CLI_DEBUGGING        0     /**< Enable debugging for CLI library. */
#define BSP_DEBUGGING        1     /**< Enable debugging for Board Support Packages. */
#define USR_DEBUGGING        0     /**< Enable debugging for USR libraries. */

#else

#define DEBUGF(debug, x)

#define USO_DEBUGGING        0
#define DEV_DEBUGGING        0
#define NET_DEBUGGING        0
#define NAP_DEBUGGING        0
#define CLI_DEBUGGING        0
#define BSP_DEBUGGING        0
#define USR_DEBUGGING        0

#endif



#if USO_DEBUGGING

#define USO_HEAP_DEBUG       1      /**< USO : enable debugging for the heap module. */
#define USO_HEAP_LIST_DEBUG  0      /**< USO : analyze the heap list. */

#else

#define USO_HEAP_DEBUG       0
#define USO_HEAP_LIST_DEBUG  0

#endif

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
