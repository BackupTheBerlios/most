/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#ifndef PC_START_H
#define PC_START_H

#include <ace/stdio.h>

/** @addtogroup init
 *
 * @{
 */

/** @defgroup start start.h
 *
 * Start
 *
 * @{
 */


/**
 * Start all services.
 */
extern void PC_start_kernel (ACE_FILE * stdout, ACE_FILE * stdin);

/** @}
 */

/** @}
 */


#endif
