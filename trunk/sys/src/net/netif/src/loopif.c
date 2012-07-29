/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

// #include "net/debug.h"
// #include "net/opt.h"
#include "net/loopif.h"
#include "net/ip.h"

static ACE_err_t
loopif_output (NET_netif_t * netif, NET_netbuf_t * p, NET_ip_addr_t * ipaddr)
{
    ++netif->tx;
    ++netif->rx;
    ACE_err_t err = NET_ip_input (netif, p);
    return err;
}

extern void
NET_loopif_init (NET_netif_t * netif)
{
    netif->output = loopif_output;
}
