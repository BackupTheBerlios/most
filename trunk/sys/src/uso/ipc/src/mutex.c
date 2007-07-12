/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/mutex.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_mutex_init (USO_mutex_t * mutex)
{
    USO_semaphore_init(mutex, 1);
}

/*------------------------------------------------------------------------*/
