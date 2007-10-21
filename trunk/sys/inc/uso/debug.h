/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_DEBUG_H
#define USO_DEBUG_H

#include "uso/log.h"

/** @defgroup debug debug.h
 *
 * Debug definitions for developing.
 *
 * Turn on debugging at compiletime.
 * @{
 */

/*------------- Definitions ------------------------------------------*/

#define DEBUGGING 1

#if DEBUGGING

/**
 * Debug function switched on and of at compiletime.
 */
#define DEBUGF(debug, x) do { if(debug){ USO_debug x; } } while(0)

#define USO_DEBUGGING        1
#define DEV_DEBUGGING        0
#define NET_DEBUGGING        1
#define NAP_DEBUGGING        0
#define CLI_DEBUGGING        0
#define BSP_DEBUGGING        0
#define USR_DEBUGGING        0

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

#define USO_HEAP_DEBUG       1
#define USO_HEAP_LIST_DEBUG  0

#else

#define USO_HEAP_DEBUG       0
#define USO_HEAP_LIST_DEBUG  0

#endif

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
