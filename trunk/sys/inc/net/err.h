/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ERR_H
#define NET_ERR_H

#include <ace/err.h>

/** @addtogroup net
 * @{
 */

/** @addtogroup err err.h
 *
 * Definitions for error constants. 
 *
 * @{
 */

#define NET_ERR         (NET_ERR_NR - 1)         /**< Not specified NET error.  */
#define NET_ERR_MEM     (NET_ERR_NR - 2)         /**< Out of memory error.  */
#define NET_ERR_BUF     (NET_ERR_NR - 3)         /**< Buffer error.  */
#define NET_ERR_VAL     (NET_ERR_NR - 4)         /**< Illegal value.  */
#define NET_ERR_ARG     (NET_ERR_NR - 5)         /**< Illegal argument.  */
#define NET_ERR_ABRT    (NET_ERR_NR - 6)         /**< Connection aborted.  */
#define NET_ERR_RST     (NET_ERR_NR - 7)         /**< Connection reset.  */
#define NET_ERR_CLSD    (NET_ERR_NR - 8)         /**< Connection closed.  */
#define NET_ERR_CONN    (NET_ERR_NR - 9)         /**< Not connected.  */
#define NET_ERR_RTE     (NET_ERR_NR - 10)         /**< Routing problem.  */
#define NET_ERR_USE     (NET_ERR_NR - 11)        /**< Address in use.  */
#define NET_ERR_BAD     (NET_ERR_NR - 12)        /**< Bad packet */
#define NET_ERR_ARP     (NET_ERR_NR - 13)        /**< ARP Request send */

/** @}
 */

/** @}
 */

#endif
