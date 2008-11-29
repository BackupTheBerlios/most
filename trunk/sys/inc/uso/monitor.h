/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_MONITOR_H
#define USO_MONITOR_H

#include "uso/mutex.h"
#include "uso/barrier.h"

/** @defgroup monitor monitor.h
 *
 * High Level Monitor.
 *
 * produce:
 *    lock (mutex);
 *    while (full) monitor (mutex,barrier);
 *    put (element);
 *    go_all (barrier);
 *    unlock (mutex);
 *
 * consume:
 *    lock (mutex);
 *    while (empty) monitor (mutex,barrier);
 *    get (element);
 *    go_all (barrier);
 *    unlock (mutex);
 *
 * @{
 */


/*-------------- Interface -----------------------------------------------*/


/**
 * Unlock the mutex and wait for a notify.
 *
 * while (!condition) monitor (mutex, barrier);.
 *
 * @param mutex : Pointer to monitor mutex.
 * @param condition : Pointer to list in which all waiting threads for condition comes true are stored.
 */
extern void USO_monitor (USO_mutex_t * mutex, USO_barrier_t * barrier);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
