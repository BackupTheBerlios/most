/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_OPT_H
#define NET_OPT_H

/** @addtogroup net
 * @{
 */

/** @defgroup opt opt.h
 *
 * Network Options.
 *
 * @{
 */

#define NET_ICMP_TTL                255 /**< ICMP time to live. */
#define NET_UDP_TTL                 255 /**< UDP time to live. */
#define NET_TCP_TTL                 255 /**< TCP time to live. */
#define NET_TCP_MSS                 128 /**< TCP maximal segment size. */
#define NET_TCP_WND                 2048 /**< TCP window size. */
#define NET_TCP_MAXRTX              12 /**< TCP max receive ??? */
#define NET_TCP_SYNMAXRTX           6 /**< TCP SYN max receive ??? */
#define NET_UDP                     1 /**< Enable UDP. */
#define NET_TCP                     0 /**< Enable TCP. */

/** @}
 */

/** @}
 */

#endif
