/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>

#include "net/stats.h"

struct NET_stats stats;

extern void
NET_stats_init (void)
{
    bzero (&stats, sizeof (struct NET_stats));
}
