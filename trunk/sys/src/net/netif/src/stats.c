/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <ace/string.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include "net/stats.h"

struct NET_stats NET_stats;

/*
static void
print_stat (char *name, struct NET_stats_proto *stat)
{
    ACE_printf ("\t%s: \t%u \t%u \t%u\n", name, stat->tx, stat->rx, stat->rx_drop);
}
*/

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    //struct NET_stats *stats = (struct NET_stats *)desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "Todo";
            entry->value.s = "todo";
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
    //print_stat ("ip", &stats->ip);
    //print_stat ("icmp", &stats->icmp);
    //print_stat ("udp", &stats->udp);
    //print_stat ("tcp", &stats->tcp);
}

static struct MFS_descriptor_op stats_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};

extern void
NET_stats_init (void)
{
    ACE_bzero (&NET_stats, sizeof (struct NET_stats));
    MFS_descriptor_t *dir = MFS_resolve("/sys/net");
    MFS_descriptor_create (dir, "stats", MFS_SYS, &stats_descriptor_op,  (MFS_represent_t *) & NET_stats);
    MFS_close_desc(dir);
}
