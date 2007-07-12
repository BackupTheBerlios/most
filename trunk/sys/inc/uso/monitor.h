/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_MONITOR_H
#define USO_MONITOR_H

#include "uso/mutex.h"

/** @defgroup monitor monitor.h
 *
 * High Level Monitor.
 *
 * produce:
 *    lock (mutex);
 *    while (full) monitor (mutex,condition);
 *    put (element);
 *    signal (contition);
 *    unlock (mutex);
 *
 * consume:
 *    lock (mutex);
 *    while (empty) monitor (mutex,condition);
 *    get (element);
 *    signal (condition);
 *    unlock (mutex);
 *
 * @{
 */


/*-------------- Interface -----------------------------------------------*/


/**
 * Unlock the mutex and wait for a notify.
 *
 * while (!condition) monitor (mutex,condition);.
 *
 * @param mutex : Pointer to monitor mutex.
 * @param condition : Pointer to list in which all waiting threads for condition comes true are stored.
 */
extern void USO_monitor (USO_mutex_t * mutex, USO_list_t * condition);

/**
 * Notify on a condition.
 *
 * If there are waiting threads, the first is wakedup.
 *
 * @param condition : Pointer to list.Pointer to list in which all waiting threads for condition comes true are stored.
 */
extern void USO_notify (USO_list_t * condition);

/**
 * Notify on a condition.
 *
 * If there are waiting threads, all are wakedup.
 *
 * @param condition : Pointer to list.Pointer to list in which all waiting threads for condition comes true are stored.
 */
 extern void USO_notify_all (USO_list_t * condition);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
