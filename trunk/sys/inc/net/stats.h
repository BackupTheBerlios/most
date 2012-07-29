/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_STATS_H
#define NET_STATS_H

#include <ace/stddef.h>

/** @addtogroup net
 * @{
 */

/** @defgroup stats stats.h
 *
 * Network Statistic.
 *
 * @{
 */

/** Statistic for each protocol. */
struct NET_stats_proto
{
    unsigned int tx;                   /**< Transmitted packets. */
    unsigned int rx;                   /**< Received packets. */
    unsigned int rx_drop;              /**< Misc receive error. */
};

/** Network Statistic. */
struct NET_stats
{
    struct NET_stats_proto ip;      /**< IP Statistic. */
    struct NET_stats_proto icmp;    /**< ICMP Statistic. */
    struct NET_stats_proto udp;     /**< UDP Statistic. */
    struct NET_stats_proto tcp;     /**< TCP Statistic. */
};

/** Network Statistic instance. */
extern struct NET_stats NET_stats;

/**
 * Initialize network statistic.
 */
extern void NET_stats_init (void);

/** @}
 */

/** @}
 */

#endif
