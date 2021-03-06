/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_DEBUG_H
#define NET_DEBUG_H

#include "uso/debug.h"

/** @addtogroup net
 * @{
 */

/** @addtogroup debug
 *
 * If NET_DEBUGGING is defined.
 *
 * @{
 */

#if NET_DEBUGGING

#define NET_NETBUF_DEBUG      1 /**< NET_DEBUGGING on */
#define NET_ETH_DEBUG         1 /**< NET_DEBUGGING on */
#define NET_ARP_DEBUG         1 /**< NET_DEBUGGING on */
#define NET_NETIF_DEBUG       1 /**< NET_DEBUGGING on */
#define NET_ICMP_DEBUG        1 /**< NET_DEBUGGING on */
#define NET_INET_DEBUG        1 /**< NET_DEBUGGING on */
#define NET_IP_DEBUG          1 /**< NET_DEBUGGING on */
#define NET_IP_HEAD_DEBUG     0 /**< NET_DEBUGGING on */
#define NET_UDP_DEBUG         1 /**< NET_DEBUGGING on */
#define NET_UDP_HEAD_DEBUG    0 /**< NET_DEBUGGING on */
#define NET_TCP_DEBUG         0 /**< NET_DEBUGGING on */

#else

#define NET_NETBUF_DEBUG      0 /**< NET_DEBUGGING off */
#define NET_ETH_DEBUG         0 /**< NET_DEBUGGING off */
#define NET_ARP_DEBUG         0 /**< NET_DEBUGGING off */
#define NET_NETIF_DEBUG       0 /**< NET_DEBUGGING off */
#define NET_ICMP_DEBUG        0 /**< NET_DEBUGGING off */
#define NET_INET_DEBUG        0 /**< NET_DEBUGGING off */
#define NET_IP_DEBUG          0 /**< NET_DEBUGGING off */
#define NET_IP_HEAD_DEBUG     0 /**< NET_DEBUGGING off */
#define NET_UDP_DEBUG         0 /**< NET_DEBUGGING off */
#define NET_UDP_HEAD_DEBUG    0 /**< NET_DEBUGGING off */
#define NET_TCP_DEBUG         0 /**< NET_DEBUGGING off */

#endif

/** @}
 */

/** @}
 */

#endif
