/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_MONITOR_H
#define USO_MONITOR_H

#include "uso/mutex.h"
#include "uso/barrier.h"

/** @addtogroup uso
 * @{
 */

/** @defgroup monitor monitor.h
 *
 * Similar to synchronized, wait and notify in Java.
 * Example shows the principle:
 *
 * produce:
 *    lock (mutex);
 *    while (full) monitor (mutex, barrier);
 *    put (element);
 *    go_all (barrier);
 *    unlock (mutex);
 *
 * consume:
 *    lock (mutex);
 *    while (empty) monitor (mutex, barrier);
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
 * Principle:
 *
 * While (!condition) monitor (mutex, barrier);.
 *
 * @param mutex : Pointer to monitor mutex.
 * @param barrier : Pointer to barrier where all threads waiting for a condition comes true are blocked.
 */
extern void USO_monitor (USO_mutex_t * mutex, USO_barrier_t * barrier);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
