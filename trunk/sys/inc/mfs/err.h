/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MFS_ERR_H
#define MFS_ERR_H

#include <ace/err.h>

/** @addtogroup mfs
 * @{
 */

/** @addtogroup err err.h
 *
 * Definitions for error constants. 
 *
 * @{
 */

#define MFS_ERR         (MFS_ERR_NR - 1)         /**< Not specified MFS error.  */
#define MFS_ERR_IN_USE  (MFS_ERR_NR - 2)         /**< Resource is in use.  */

/** @}
 */

/** @}
 */

#endif
