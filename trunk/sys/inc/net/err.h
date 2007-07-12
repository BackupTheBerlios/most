/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_ERR_H
#define NET_ERR_H

/** @defgroup err err.h
 *
 * Definitions for error constants. 
 *
 * @{
 */

/**
 * Network error type
 */
typedef int NET_err_t;

#define NET_ERR_OK    0         /**< No error, everything OK. */
#define NET_ERR_MEM  -1         /**< Out of memory error.  */
#define NET_ERR_BUF  -2         /**< Buffer error.  */
#define NET_ERR_ABRT -3         /**< Connection aborted.  */
#define NET_ERR_RST  -4         /**< Connection reset.  */
#define NET_ERR_CLSD -5         /**< Connection closed.  */
#define NET_ERR_CONN -6         /**< Not connected.  */
#define NET_ERR_VAL  -7         /**< Illegal value.  */
#define NET_ERR_ARG  -8         /**< Illegal argument.  */
#define NET_ERR_RTE  -9         /**< Routing problem.  */
#define NET_ERR_USE  -10        /**< Address in use.  */
#define NET_ERR_BAD  -11        /**< Bad packet */
#define NET_ERR_ARP  -12        /**< ARP Request send */

/** @}
 */

#endif
