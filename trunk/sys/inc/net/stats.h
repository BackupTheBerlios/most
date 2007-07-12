/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_STATS_H
#define NET_STATS_H

#include <ace/stddef.h>

/** @defgroup stats stats.h
 *
 * Network Statistic.
 *
 * @{
 */

/** ? */
struct NET_stats_proto
{
    u16_t tx;                   /**< Transmitted packets. */
    u16_t rx;                   /**< Received packets. */
    u16_t rx_drop;              /**< Misc error. */
};

/** ? */
struct NET_stats
{
    struct NET_stats_proto ip;      /**< IP Statistic */
    struct NET_stats_proto icmp;    /**< ICMP Statistic */
    struct NET_stats_proto udp;     /**< UDP Statistic */
    struct NET_stats_proto tcp;     /**< TCP Statistic */
};

/** Net Statistic */
extern struct NET_stats stats;

/**
 * Initialize Statistic
 */
extern void NET_stats_init (void);

/** @}
 */

#endif
